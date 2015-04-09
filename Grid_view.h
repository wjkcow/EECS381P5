#ifndef GRID_VIEW_H
#define GRID_VIEW_H
#include "View.h"

#include <vector>
#include <map>
#include <string>
struct Point;

class Grid_view : public View {
public:
    // update the location of the ship
    void update_location(const std::string& name, Point location) override;
    // remove the info of ship from the containers
    void update_remove(const std::string& name) override;
    // draw this location view
    void draw() override;
    
protected:
    struct Obj_index{
        std::string name;
        int    idx;
        int    idy;
    };
    virtual std::vector<Obj_index> get_indexes(const std::map<std::string, Point>& plot_objectes) = 0;
    virtual std::string get_placeholder() = 0;
    virtual int         get_grid_size_x() = 0;
    virtual int         get_grid_size_y() = 0;
    
    // the function prints the y label at the left of each line
    virtual void print_y_label(int line) = 0;
    
    // this function prints all the x label in the bottom of the grid
    virtual void print_x_label() = 0;
    
    // Calculate the cell subscripts corresponding to the location parameter, using the
    // current size, scale, and origin of the display.
    // Return true if the location is within the map, false if not
    bool get_subscripts(int &ix, int &iy, Point location, int size,
                        double scale, Point origin);
    
private:
    // put the object name at the index of grid
    void set_object_at_grid(const Obj_index &object_index,
                            std::vector<std::vector<std::string>> &grid_map);
    std::map<std::string, Point> plot_objects;
};

#endif
