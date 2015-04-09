#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <iomanip>
struct Point;

class View {
public:
    virtual ~View();
    
    // update functions
    // If the name is already present,the new info replaces the previous one.
    // update the object's location
    virtual void update_location(const std::string& name, Point location);
    // update ship's speed
    virtual void update_ship_speed(const std::string& name, double value){}
    // update ship's course
    virtual void update_ship_course(const std::string& name, double value){}
    // update ship's fuel
    virtual void update_ship_fuel(const std::string& name, double value){}
    
    // Remove the name and its location; no error if the name is not present.
    virtual void update_remove(const std::string& name) = 0;
    
    // prints out the current map
    virtual void draw() = 0;
protected:
    // RAII class to save the state of the cout
    class Cout_saver{
    public:
        Cout_saver();
        ~Cout_saver();
    private:
        std::ios::fmtflags old_settings;
        int old_precision;
    };
};

#endif
