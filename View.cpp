#include "View.h"
#include "Utility.h"
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
/* *** Use this function to calculate the subscripts for the cell. */

/* *** This code assumes the specified private member variables. */


// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View::get_subscripts(int &ix, int &iy, Point location)
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

View::View(){
    cout << "View constructed" << endl;
}

View::~View(){
    cout << "View destructed" << endl;
}

void View::update_location(const string& name, Point location){
    plot_objects[name] = location;
}

void View::update_remove(const string& name){
    plot_objects.erase(name);
}

void View::draw(){
    ios::fmtflags old_settings = cout.flags();
    int old_precision = static_cast<int>(cout.precision());
    
    vector<vector<string>> map{static_cast<size_t>(size),
        vector<string>{static_cast<size_t>(size), ". "}};
    cout <<  "Display size: " << size << ", scale: " << scale <<
    ", origin: " << origin << endl;
    int x,y;
    string out_of_maps;
    for (auto &plot_pair : plot_objects) {
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
    
    cout.flags(old_settings);
    cout.precision(old_precision);
}

void View::clear(){
    plot_objects.clear();
}

void View::set_size(int size_){
    if (size_ <= min_size_c) {
        throw Error("New map size is too small!");
    }
    if(size_ > max_size_c){
        throw Error("New map size is too big!");
    }
    size = size_;
}

void View::set_scale(double scale_){
    if (scale_ <= 0.) {
        throw Error("New map scale must be positive!");
    }
    scale = scale_;
}

void View::set_origin(Point origin_){
    origin = origin_;
}

void View::set_defaults(){
    size = default_size_c;
    scale = default_scale_c;
    origin = Point{default_origin_c, default_origin_c};
}

