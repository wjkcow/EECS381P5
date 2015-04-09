#include "Bridge_view.h"
#include "Geometry.h"
#include "Navigation.h"
#include <iostream>

using namespace std;

const double max_view_range_c = 20.;
const double min_view_range_c = 0.005;

const int    grid_x_size_c = 19;
const int    grid_y_size_c = 3;
const int    obj_y_pos_c  = 0;
const double default_scale_c = 10.;
const double default_origin_c = -90.;

Bridge_view::Bridge_view(const string& own_ship_name_) :
own_ship_name{own_ship_name_}{
}

// update functions
// If the name is already present,the new info replaces the previous one.
// update the object's location
void Bridge_view::update_location(const std::string& name, Point location){
    if (name == own_ship_name) {
        own_ship_position = location;
    } else {
        Grid_view::update_location(name, location);
    }
}

// Remove the name and its location; no error if the name is not present.
void Bridge_view::update_remove(const std::string& name){
    if (name == own_ship_name) {
        is_sunk = true;
    } else {
        Grid_view::update_remove(name);
    }
}
// update ship's course
void Bridge_view::update_ship_course(const std::string& name, double value){
    if (name == own_ship_name) {
        heading = value;
    }
}

void Bridge_view::draw(){
    if (is_sunk) {
        cout << "Bridge view from " << own_ship_name << " sunk at "
        << own_ship_position << endl;
        Grid_view::draw();
    } else {
        cout << "Bridge view from " << own_ship_name << " position "
        << own_ship_position << " heading " << heading << endl;
        Grid_view::draw();
    }
}

std::vector<Grid_view::Obj_index> Bridge_view::get_indexes(const std::map<std::string, Point>& plot_objects){
    if (is_sunk) {
        return vector<Grid_view::Obj_index>{};
    }
    vector<Grid_view::Obj_index> obj_idx;
    for (const auto & loc_pair: plot_objects) {
        Compass_position cp{own_ship_position,loc_pair.second};
        if (cp.range > max_view_range_c || cp.range < min_view_range_c) {
            continue;
        }
        double view_angle = cp.bearing - heading;;
        if (view_angle < -180) {
            view_angle += 360;
        }
        if (view_angle > 180) {
            view_angle -= 360;
        }
        int x;
        if(get_subscribe(x, view_angle)){
            obj_idx.push_back(Grid_view::Obj_index {loc_pair.first, x, obj_y_pos_c});
        }
    }
    return obj_idx;
}

std::string Bridge_view::get_placeholder(){
    if (is_sunk) {
        return "w-";
    } else {
        return ". ";
    }
}

int Bridge_view::get_grid_size_x(){
    return grid_y_size_c;
}

int Bridge_view::get_grid_size_y(){
    return grid_x_size_c;
}


void Bridge_view::print_y_label(int){
    cout << "     ";
}

void Bridge_view::print_x_label(){
    cout << "   -90   -60   -30     0    30    60    90" << endl;
}

// this function get the subscribe for the view
bool Bridge_view::get_subscribe(int &x,double view_angle){
    int y;
    return get_subscripts(x, y, Point{view_angle, view_angle},
                          grid_x_size_c, default_scale_c, Point{default_origin_c, default_origin_c});
}

