#ifndef MAP_VIEW_H
#define MAP_VIEW_H
#include "Grid_view.h"

#include "Geometry.h"

class Map_view : public Grid_view {
public:
    Map_view();
    // modify the display parameters
    // if the size is out of bounds will throw Error("New map size is too big!")
    // or Error("New map size is too small!")
    void set_size(int size_);
    
    // If scale is not postive, will throw Error("New map scale must be positive!");
    void set_scale(double scale_);
    // any values are legal for the origin
    void set_origin(Point origin_);
    
    // set the parameters to the default values
    void set_defaults();
    // prints out the current map
    void draw() override;
protected:
    std::vector<Grid_view::Obj_index>
    get_indexes(const std::map<std::string, Point>&
                plot_objectes) override;
    std::string get_placeholder() override;
    int         get_grid_size_x() override;
    int         get_grid_size_y() override;
    // the function prints the y label at the left of each line
    void print_y_label(int line) override;
    
    // this function prints all the x label in the bottom of the grid
    void print_x_label() override;
    
private:
    int size;			// current size of the display
    double scale;		// distance per cell of the display
    Point origin;		// coordinates of the lower-left-hand corner
};

#endif /* defined(__EECS381P5__Map_view__) */
