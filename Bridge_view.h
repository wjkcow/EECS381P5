#ifndef BRIDGE_VIEW_H
#define BRIDGE_VIEW_H
#include "Grid_view.h"
#include "Geometry.h"

#include <string>

class Bridge_view : public Grid_view{
public:
    Bridge_view(const std::string& own_ship_name_);
    // update functions
    // If the name is already present,the new info replaces the previous one.
    // update the object's location
    void update_location(const std::string& name, Point location);
    // Remove the name and its location; no error if the name is not present.
    void update_remove(const std::string& name);    
    // update ship's course
    void update_ship_course(const std::string& name, double value);
    
protected:
    void print_header();
    std::vector<Grid_view::Obj_index>
                get_indexes(const std::map<std::string, Point>&
                plot_objectes) override;
    std::string get_placeholder() override;
    std::string get_multiple_obj_placehodler();
    int         get_grid_size_x() override;
    int         get_grid_size_y() override;
    // the function prints the y label at the left of each line
    void print_y_label(int line) override;
    
    // this function prints all the x label in the bottom of the grid
    void print_x_label() override;
    
private:
    // this function get the subscribe for the view, return false if it out of
    //range
    bool get_subscribe(int &x,double view_angle);
    std::string own_ship_name;
    bool is_sunk;
    double heading;
    Point own_ship_position;
};

#endif
