#ifndef VIEW_H
#define VIEW_H

#include "Geometry.h"
class View {
public:
    virtual ~View(){};
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
    // default behavior is doing nothing
    virtual void update_location(const std::string& name, Point location){
    }
    
    // update the sailing_data of a specifc function;
    // default behavior is doing nothing
    virtual void update_sailing_data(const std::string& name, double fuel, double course, double speed){};
	
	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name) = 0;
	
	// prints out the current map
	virtual void draw() = 0;
	
	// Discard the saved information - drawing will show only a empty pattern
    virtual void clear(){};
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
    virtual void set_size(int size_){};
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
    virtual void set_scale(double scale_){};
	
	// any values are legal for the origin
    virtual void set_origin(Point origin_){};
	
	// set the parameters to the default values
    virtual void set_defaults(){};
};

#endif
