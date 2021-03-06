#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <functional>
#include <memory>
#include <string>
#include <vector>
class  Model;
class View;
class Map_view;
class Bridge_view;
class Sailing_data_view;
class Ship;
class Island;
struct Point;
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

class Controller {
public:	
	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
private:
    std::shared_ptr<Map_view> map_view;
    std::shared_ptr<Sailing_data_view> sailing_data_view;
    std::map<std::string, std::shared_ptr<Bridge_view>> bridge_views;
    std::vector<std::shared_ptr<View>> all_views;
    
    // handler for all the ship commands
    void ship_cmd(const std::string& cmd);
    // handler for model and view commands
    void view_model_cmd(const std::string& cmd);
    
    
    // handlers to the view comands:
    // restore the default setting of the map
    void view_cmd_default() const;
    // read a single integer for the size of the map
    void view_cmd_size() const;
    // read a double value for the scale of the map
    void view_cmd_zoom() const;
    // read a pair of double values for the (x,y) origin of the map
    void view_cmd_pan() const;
    // tell the view to draw the map
    void view_cmd_show() const;
    
    // new commands for multiple views
    void open_map_view();
    void close_map_view();
    void open_sailing_view();
    void close_sailing_view();
    void open_bridge_view();
    void close_bridge_view();
    // helper function to add view
    void attach_view(std::shared_ptr<View> sp);
    // helper function to remove view
    void detach_view(std::shared_ptr<View> sp);
    // handlers to the model commands:
    // have all the objects describe themselves
    void model_cmd_status() const;
    // update all the objects
    void model_cmd_go() const;
    /* read a name for a new ship; throw an error if it is too short, and
    have Model check it for validity, and throw an Error if the name is
    already in use. Then read a type name for the new ship, and a pair of 
    doubles for its initial position. Create the new ship using 
    the Ship_factory and add it to the Model.*/
    void model_cmd_create() const;
    
    // handlers for ship commands:
    // read a compass heading and a speed for the ship to set course and speed
    void ship_course(std::shared_ptr<Ship> ship_ptr) const;
    // read an (x,y) position and then a speed for the Ship to set destination postion and speed to go to
    void ship_position(std::shared_ptr<Ship> ship_ptr) const;
    // read an Island name and a speed. Go the the position of the island
    void ship_destination(std::shared_ptr<Ship> ship_ptr) const;
    // load at the island name
    void ship_load_at(std::shared_ptr<Ship> ship_ptr) const;
    // unload at the island name
    void ship_unload_at(std::shared_ptr<Ship>  ship_ptr) const;
    // dock at the island name
    void ship_dock_at(std::shared_ptr<Ship>  ship_ptr) const;
    // attack at the sheep
    void ship_attack(std::shared_ptr<Ship>  ship_ptr) const;
    // refuel the ship
    void ship_refuel(std::shared_ptr<Ship>  ship_ptr) const;
    // stop the ship
    void ship_stop(std::shared_ptr<Ship>  ship_ptr) const;
    // stop attack
    void ship_stop_attack(std::shared_ptr<Ship>  ship_ptr) const;
    


    // clear the data before quit
    void clear();
    // helper functions:
    std::shared_ptr<Ship> get_ship() const;
    std::shared_ptr<Island> get_island() const;
    
    // helper function for reading
    double read_double() const;
    Point  read_point() const;
    double read_compass_heading() const;
    double read_speed() const;
    
    // map for all the ship commands //NOTE: 
    std::map<std::string, std::function<void(std::shared_ptr<Ship>)>> ship_cmds =
    {
        {"course",
            std::bind(&Controller::ship_course, this, std::placeholders::_1)},
        {"position",
            std::bind(&Controller::ship_position, this, std::placeholders::_1)},
        {"destination",
            std::bind(&Controller::ship_destination, this, std::placeholders::_1)},
        {"load_at",
            std::bind(&Controller::ship_load_at, this, std::placeholders::_1)},
        {"unload_at",
            std::bind(&Controller::ship_unload_at, this, std::placeholders::_1)},
        {"dock_at",
            std::bind(&Controller::ship_dock_at, this, std::placeholders::_1)},
        {"attack",
            std::bind(&Controller::ship_attack, this, std::placeholders::_1)},
        {"refuel",
            std::bind(&Controller::ship_refuel, this, std::placeholders::_1)},
        {"stop",
            std::bind(&Controller::ship_stop, this, std::placeholders::_1)},
        {"stop_attack",
            std::bind(&Controller::ship_stop_attack, this, std::placeholders::_1)}
    };
    
    // map for all the view or model commands;
    std::map<std::string, std::function<void(void)>> view_model_cmds =
    {
        {"default", std::bind(&Controller::view_cmd_default, this)},
        {"size",    std::bind(&Controller::view_cmd_size, this)},
        {"zoom",    std::bind(&Controller::view_cmd_zoom, this)},
        {"pan",     std::bind(&Controller::view_cmd_pan, this)},
        {"show",    std::bind(&Controller::view_cmd_show, this)},
        {"open_map_view", std::bind(&Controller::open_map_view, this)},
        {"close_map_view",    std::bind(&Controller::close_map_view, this)},
        {"open_sailing_view",    std::bind(&Controller::open_sailing_view, this)},
        {"close_sailing_view",     std::bind(&Controller::close_sailing_view, this)},
        {"open_bridge_view",    std::bind(&Controller::open_bridge_view, this)},
        {"close_bridge_view",    std::bind(&Controller::close_bridge_view, this)},
        {"status",  std::bind(&Controller::model_cmd_status, this)},
        {"go",      std::bind(&Controller::model_cmd_go, this)},
        {"create",  std::bind(&Controller::model_cmd_create, this)}
    };
    constexpr static int min_name_len = 2;
    constexpr static double max_compass_heading = 360.;
};

#endif
