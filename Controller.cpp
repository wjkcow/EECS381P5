#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"

#include <iostream>
#include <map>
#include <functional>
using namespace std;
using namespace placeholders;

Controller::Controller(){
    cout << "Controller constructed" << endl;
}

Controller::~Controller(){
    cout << "Controller destructed" << endl;
}

void Controller::run(){
    view = new View{};
    g_Model_ptr->attach(view);


    map<string, function<void(void)>> view_model_cmds = {
        {"default", bind(&Controller::view_cmd_default, this)},
        {"size",    bind(&Controller::view_cmd_default, this)},
        {"zoom",    bind(&Controller::view_cmd_default, this)},
        {"pan",     bind(&Controller::view_cmd_default, this)},
        {"show",    bind(&Controller::view_cmd_default, this)},
        {"status",  bind(&Controller::view_cmd_default, this)},
        {"go",      bind(&Controller::view_cmd_default, this)},
        {"create",  bind(&Controller::view_cmd_default, this)}
    };
    map<string, function<void(Ship*)>> ship_cmds = {
        {"course",      bind(&Controller::ship_course, this, _1)},
        {"position",    bind(&Controller::ship_position, this, _1)},
        {"destination", bind(&Controller::ship_destination, this, _1)},
        {"load_at",     bind(&Controller::ship_load_at, this, _1)},
        {"unload_at",   bind(&Controller::ship_unload_at, this, _1)},
        {"dock_at",     bind(&Controller::ship_dock_at, this, _1)},
        {"attack",      bind(&Controller::ship_attack, this, _1)},
        {"refuel",      bind(&Controller::ship_refuel, this, _1)},
        {"stop",        bind(&Controller::ship_stop, this, _1)},
        {"stop_attack", bind(&Controller::ship_stop_attack, this, _1)}
    };
    string cmd_word;
    string ship_cmd_word;
    while (true) {
        cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
        cin >> cmd_word;
        if (cmd_word == "quit") {
            g_Model_ptr->detach(view);
            delete view;
            cout << "Done" << endl;
            return;
        } else if(g_Model_ptr->is_ship_present(cmd_word) &&
                  ship_cmds.count(ship_cmd_word)){
            cin >> ship_cmd_word;
            ship_cmds[ship_cmd_word](g_Model_ptr->get_ship_ptr(cmd_word));
        } else if(view_model_cmds.count(cmd_word)){
            view_model_cmds[cmd_word]();
        } else {
            throw Error("Unrecognized command!");
        }
    }
}

void Controller::view_cmd_default() const{
    view->set_defaults();
}

void Controller::view_cmd_size() const{
    int sz;
    if(!(cin >> sz)){
        throw Error("Expected an integer!");
    }
    view->set_size(sz);
}

void Controller::view_cmd_zoom() const{
    view->set_scale(read_double());
}

void Controller::view_cmd_pan() const{
    view->set_origin(read_point());
}

void Controller::view_cmd_show() const{
    view->draw();
}

void Controller::model_cmd_status() const{
    g_Model_ptr->describe();
}

void Controller::model_cmd_go() const{
    g_Model_ptr->update();
}

void Controller::model_cmd_create() const{
    string name;
    cin >> name;
    if (name.length() < min_name_len) {
        throw Error("Name is too short!");
    }
    if (g_Model_ptr->is_name_in_use(name)) {
        throw Error("Name is already in use!");
    }
    
    string type;
    cin >> type;
    g_Model_ptr->add_ship(create_ship(name, type, read_point()));
    
}

void Controller::ship_course(Ship* ship_ptr) const{
    ship_ptr->set_course_and_speed(read_compass_heading(), read_speed());
}

void Controller::ship_position(Ship* ship_ptr) const{
    ship_ptr->set_destination_position_and_speed(read_point(), read_speed());
}

void Controller::ship_destination(Ship* ship_ptr) const{
    ship_ptr->set_destination_position_and_speed(get_island()->get_location(),
                                                 read_speed());
}

void Controller::ship_load_at(Ship* ship_ptr) const{
    ship_ptr->set_load_destination(get_island());
}

void Controller::ship_unload_at(Ship* ship_ptr) const{
    ship_ptr->set_unload_destination(get_island());
}

void Controller::ship_dock_at(Ship* ship_ptr) const{
    ship_ptr->dock(get_island());
}

void Controller::ship_attack(Ship* ship_ptr) const{
    ship_ptr->attack(get_ship());
}

void Controller::ship_refuel(Ship* ship_ptr) const{
    ship_ptr->refuel();
}

void Controller::ship_stop(Ship* ship_ptr) const{
    ship_ptr->stop();
}

void Controller::ship_stop_attack(Ship* ship_ptr) const{
    ship_ptr->stop_attack();
}

Ship* Controller::get_ship() const{
    string name;
    cin >> name;
    return g_Model_ptr->get_ship_ptr(name);
}

Island* Controller::get_island() const{
    string name;
    cin >> name;
    return g_Model_ptr->get_island_ptr(name);
}

double Controller::read_double() const{
    double read_d;
    if(!(cin >> read_d)){
        throw Error("Expected a double!");
    }
    return read_d;
}

Point Controller::read_point() const{
    return Point{read_double(), read_double()};
}

double Controller::read_compass_heading() const{
    double compass_heading = read_double();
    if (compass_heading < 0. || compass_heading >= 360.) {
        throw Error("Invalid heading entered!");
    }
    return compass_heading;
}

double Controller::read_speed() const{
    double speed = read_double();
    if (speed < 0.) {
        throw Error("Negative speed entered!");
    }
    return speed;
}

