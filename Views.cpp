#include "Views.h"
#include "Utility.h"
#include "Navigation.h"
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <iterator>

using namespace std;

/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */


// Calculate the cell subscripts corresponding to the supplied location parameter,
// using the current size, scale, and origin of the display.
// This function assumes that origin is a  member variable of type Point,
// scale is a double value, and size is an integer for the number of rows/columns
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Graph_view::get_subscripts(int &ix, int &iy, Point location)
{
    // adjust with origin and scale
    Cartesian_vector subscripts = (location - origin) / scale;
    // truncate coordinates to integer after taking the floor
    // floor function will produce integer smaller than even for negative values,
    // so - 0.05 => -1., which will be outside the array.
    ix = int(floor(subscripts.delta_x));
    iy = int(floor(subscripts.delta_y));
    // if out of range, return false
    if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
        return false;
    }
    else
        return true;
}

Graph_view::Graph_view(int size_, double scale_, double origin_):
size{size_}, scale{scale_}, origin{origin_, origin_}{}

void Graph_view::update_location(const string& name, Point location){
    plot_objects[name] = location;
}

void Graph_view::update_remove(const string& name){
    plot_objects.erase(name);
}


void Graph_view::clear(){
    plot_objects.clear();
}


Map_view::Map_view():Graph_view{default_size_c,default_scale_c,
    default_origin_c}{
}

Map_view::Cout_saver::Cout_saver(){
    old_settings = cout.flags();
    old_precision = static_cast<int>(cout.precision());
}
Map_view::Cout_saver::~Cout_saver(){
    cout.flags(old_settings);
    cout.precision(old_precision);
}

void Map_view::draw(){
    Cout_saver cout_saver{};
    vector<vector<string>> map{static_cast<size_t>(size),
        vector<string>{static_cast<size_t>(size), ". "}};
    cout <<  "Display size: " << size << ", scale: " << scale <<
    ", origin: " << origin << endl;
    int x,y;
    string out_of_maps;
    for (auto &plot_pair : get_locations()) {
        if (!get_subscripts(x, y, plot_pair.second)) {
            if (!out_of_maps.empty()) {
                out_of_maps += ", ";
            }
            out_of_maps += plot_pair.first;
        } else if(map[x][y] == ". "){
            map[x][y] = plot_pair.first.substr(0, plot_wide_c);
        } else {
            map[x][y] = "* ";
        }
    }
    if (!out_of_maps.empty()) {
        cout << out_of_maps << " outside the map" << endl;
    }
    
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(0);
    int label_count = size % label_gap ? size / label_gap + 1 :
    size / label_gap;
    double y_label = origin.y +
    scale * label_gap * (label_count - 1);
    for (int y = 0; y < size; y++) {
        if ((size - y)%label_gap == 1) {
            cout << setw(output_y_label_w_c) << y_label << " ";
            y_label -= scale * label_gap;
        } else {
            cout << "     ";
        }
        for (int x = 0; x < size; x ++) {
            cout << map[x][size - y - 1];
        }
        cout << endl;
    }
    double x_label = origin.x;
    
    
    for (int x = 0; x < label_count; x++) {
        cout << setw(output_x_label_w_c) << x_label;
        x_label += scale * label_gap;
    }
    cout << endl;
}


void Map_view::set_size(int size_){
    if (size_ <= min_size_c) {
        throw Error("New map size is too small!");
    }
    if(size_ > max_size_c){
        throw Error("New map size is too big!");
    }
    size = size_;
}

void Map_view::set_scale(double scale_){
    if (scale_ <= 0.) {
        throw Error("New map scale must be positive!");
    }
    scale = scale_;
}

void Map_view::set_origin(Point origin_){
    origin = origin_;
}

void Map_view::set_defaults(){
    size = default_size_c;
    scale = default_scale_c;
    origin = Point{default_origin_c, default_origin_c};
}

// BELOW IS FUNCTIONS OF BRIDGE VIEW
Bridge_view::Bridge_view(const std::string& name_): Graph_view{default_size_c, default_scale_c, default_origin_c},own_ship_name{name_},
    is_sunk{false}{
}

void Bridge_view::update_sailing_data(const string& name, double, double course,
                                      double){
    if (name == own_ship_name) {
        heading = course;
    }
}
void Bridge_view::update_remove(const std::string& name){
    if (name == own_ship_name) {
        is_sunk = true;
    } else {
        Graph_view::update_remove(name);
    }
}

void Bridge_view::draw(){

    if (is_sunk) {
        cout << "Bridge view from " << own_ship_name << " sunk at "
             << get_locations().at(own_ship_name) << endl;
        cout << "     w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-"<< endl;
        cout << "     w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-"<< endl;
        cout << "     w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-w-"<< endl;
        print_label();
    } else {
        cout << "Bridge view from " << own_ship_name << " position "
        << get_locations().at(own_ship_name) << " heading " << heading << endl;
        
        cout << "     . . . . . . . . . . . . . . . . . . . " << endl;
        cout << "     . . . . . . . . . . . . . . . . . . . " << endl;

        vector<string> bridge_view_data{default_size_c, ". "};
        const std::map<std::string, Point>& locations =
                                                Graph_view::get_locations();
        Point own_ship_loc = locations.at(own_ship_name);
        for (const auto & loc_pair: locations) {
            Compass_position cp{own_ship_loc,loc_pair.second};
            if (cp.range <= max_view_range && cp.range >= min_view_range) {
                double view_angle = cp.bearing - heading;
                if (view_angle < -180) {
                    view_angle += 360;
                }
                if (view_angle > 180) {
                    view_angle -= 360;
                }
                int x;
                if(get_subscribe(x, view_angle)){
                    if (bridge_view_data[x] == ". ") {
                        bridge_view_data[x] = loc_pair.first.substr(0, 2);
                    } else {
                        bridge_view_data[x] = "**";
                    }
                }
            }
        }
        cout << "     ";
        copy(bridge_view_data.begin(), bridge_view_data.end(),
             ostream_iterator<string>(cout));
        cout << endl;
        print_label();
    }
}
bool Bridge_view::get_subscribe(int &x, double view_angle){
    int y;
    return get_subscripts(x, y, Point{view_angle, view_angle});
}

void Bridge_view::print_label(){
    cout << "   -90   -60   -30     0    30    60    90" << endl;
}

// BELOW IS FUNCTIONS FOR SAILING_DATA VIEW

void Sailing_data_view::update_sailing_data(const string& name, double fuel,
                                            double course, double speed){
    data[name] = Sailing_data_t{fuel, course, speed};
}

void Sailing_data_view::update_remove(const string& name){
    data.erase(name);
}

void Sailing_data_view::draw(){
    cout << "----- Sailing Data -----" << endl;
    cout << setw(10) << "Ship" << setw(10) << "Fuel" <<  setw(10) <<"Course"
         << setw(10) << "Speed" << endl;
    for (auto& data_pair: data) {
        cout << setw(10) << data_pair.first << setw(10) << data_pair.second.fuel
            <<  setw(10) << data_pair.second.course
            << setw(10) << data_pair.second.speed << endl;
    }

}

void Sailing_data_view::clear(){
    data.clear();
}

