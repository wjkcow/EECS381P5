#include "Grid_view.h"

#include "Navigation.h"
#include "Geometry.h"
#include <cmath>
#include <iostream>
#include <functional>
#include <cassert>

using namespace std;
using namespace std::placeholders;

// consts for all views
const string grid_multiple_obj_c = "**";
const int    grid_size_c = 2;

void Grid_view::update_location(const string& name, Point location){
    plot_objects[name] = location;
}

void Grid_view::update_remove(const string&name){
    plot_objects.erase(name);
}

void Grid_view::draw(){
    // first clear the grid_map
    vector<vector<string>> grid_map{static_cast<size_t>(get_grid_size_x()),
        {static_cast<size_t>(get_grid_size_y()), get_placeholder()}};
    
    // get the indexes of each objects from their positions
    auto objects_idxes = get_indexes(plot_objects);
    
    // set the grid map
    for_each(objects_idxes.begin(), objects_idxes.end(), bind(&Grid_view::set_object_at_grid,this, _1, ref(grid_map)));
    
    for (int i = 0; i < static_cast<int>(grid_map.size()); i ++) {
        print_y_label(i);
        copy(grid_map[grid_map.size() - i - 1].begin(),
             grid_map[grid_map.size() - i - 1].end(),
             ostream_iterator<string>{cout});
        cout << endl;
    }
    print_x_label();
}

// Calculate the cell subscripts corresponding to the supplied location parameter,
// using the current size, scale, and origin of the display.
// This function assumes that origin is a  member variable of type Point,
// scale is a double value, and size is an integer for the number of rows/columns
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Grid_view::get_subscripts(int &ix, int &iy, Point location,
                               int size, double scale, Point origin)
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

void Grid_view::set_object_at_grid(const Obj_index &object_index,
                                   vector<vector<string>> &grid_map){
    assert(object_index.idy < static_cast<int>(grid_map.size()));
    assert(object_index.idx < static_cast<int>(grid_map[0].size()));
    assert(object_index.name.length() > grid_size_c);
    if (grid_map[object_index.idy][object_index.idx] == get_placeholder()) {
        grid_map[object_index.idy][object_index.idx] =
        object_index.name.substr(0, grid_size_c);
    } else {
        grid_map[object_index.idy][object_index.idx] = grid_multiple_obj_c;
    }
}

