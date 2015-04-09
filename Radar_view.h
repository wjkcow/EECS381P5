#ifndef RADAR_VIEW_H
#define RADAR_VIEW_H
#include "Bridge_view.h"

class Radar_view : public Bridge_view{
public:
    Radar_view(const std::string& own_ship_name_);
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
    
};
#endif
