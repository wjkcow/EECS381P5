#ifndef SAILING_DATA_VIEW_H
#define SAILING_DATA_VIEW_H

#include "View.h"

#include <map>
class Sailing_data_view: public View{
public:
    // update ship's speed
    void update_ship_speed(const std::string& name, double speed);
    // update ship's course
    void update_ship_course(const std::string& name, double course);
    // update ship's fuel
    void update_ship_fuel(const std::string& name, double fuel);
    
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

#endif /* defined(__EECS381P5__Sailing_data_view__) */
