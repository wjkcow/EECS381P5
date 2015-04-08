#ifndef VIEWS
#define VIEWS

#include "View.h"
#include <string>
#include <map>
#include <iostream> //NOTE: include<iosfwd>

struct Point;

// this is the kind of view which will have graphic representation,
// thus the location of every name will be remembered
// it is still abstract as draw is not defined
class Graph_view : public View{
public:
    Graph_view(int size_, double scale_, double origin_);
    // Save the supplied name and location for future use in a draw() call
    // If the name is already present,the new location replaces the previous one.
    void update_location(const std::string& name, Point location) override;
    // Remove the name and its location; no error if the name is not present.
    void update_remove(const std::string& name) override;
    // Discard the saved information - drawing will show only a empty pattern
    void clear() override;
    
protected:
    const std::map<std::string, Point>& get_locations() const{
        return plot_objects;
    }
    // Calculate the cell subscripts corresponding to the location parameter, using the
    // current size, scale, and origin of the display.
    // Return true if the location is within the map, false if not
    bool get_subscripts(int &ix, int &iy, Point location);
    int size;			// current size of the display
    double scale;		// distance per cell of the display
    Point origin;		// coordinates of the lower-left-hand corner
private:
    std::map<std::string, Point> plot_objects;
};



class Map_view : public Graph_view {
public:
    Map_view();
    // prints out the current map
    void draw() override;
    

    // modify the display parameters
    // if the size is out of bounds will throw Error("New map size is too big!")
    // or Error("New map size is too small!")
    void set_size(int size_) override;
    
    // If scale is not postive, will throw Error("New map scale must be positive!");
    void set_scale(double scale_) override;
    // any values are legal for the origin
    void set_origin(Point origin_) override;
    
    // set the parameters to the default values
    void set_defaults() override;
				
private:
    //RAII class for manage cout setting
    class Cout_saver{ //NOTE: implementation 
    public:
        Cout_saver();
        ~Cout_saver();
    private:
        std::ios::fmtflags old_settings;
        int old_precision;
    };

    constexpr static int default_size_c = 25;
    constexpr static double default_scale_c = 2.;
    constexpr static double default_origin_c = -10.;
    constexpr static int max_size_c = 30; // size <= 30
    constexpr static int min_size_c = 6;  // size > 6
    // consts to help plot the map
    constexpr static int plot_wide_c = 2;
    constexpr static int output_y_label_w_c = 4;
    constexpr static int output_x_label_w_c = 6;
    constexpr static int label_gap = 3;
};

class Bridge_view: public Graph_view{
public:
    Bridge_view(const std::string& name_);
    void update_sailing_data(const std::string& name, double fuel, double course, double speed) override;
    void update_remove(const std::string& name) override;
    void draw() override;
private:
    // this function get the subscribe for the view
    bool get_subscribe(int &x,double view_angle);
    void print_label();
    std::string own_ship_name;
    bool is_sunk;
    double heading;
    constexpr static double max_view_range = 20.;
    constexpr static double min_view_range = 0.005;
    
    constexpr static int default_size_c = 19;
    constexpr static double default_scale_c = 10.;
    constexpr static double default_origin_c = -90.;
};

class Sailing_data_view: public View{
public:
    void update_sailing_data(const std::string& name, double fuel, double course, double speed) override;
    void update_remove(const std::string& name) override;
    void draw() override;
    void clear();
private:
    struct Sailing_data_t {
        double fuel;
        double course;
        double speed;
    };
    using Sailing_Datas = std::map<std::string, Sailing_data_t>;
    Sailing_Datas data;
};

#endif
