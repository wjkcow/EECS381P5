#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Views.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
using namespace placeholders;

void Controller::run(){
    string first_cmd_word;
    while (true) {
        try {
            cout << "\nTime " << Model::get_instance().get_time() << ": Enter command: ";
            cin >> first_cmd_word;
            if (first_cmd_word == "quit") {
                clear();
                return;
            } else if(Model::get_instance().is_ship_present(first_cmd_word)){
                ship_cmd(first_cmd_word);
            } else {
                view_model_cmd(first_cmd_word);
            }
        } catch (Error &e) {
            cout << e.what() << endl;
            cin.clear();
            while(cin.good() && cin.get() != '\n');
        } catch (exception &e){
            cout << e.what() << endl;
            clear();
            return;
        }
    }
}

void Controller::ship_cmd(const string& ship_name){
    string ship_cmd_word;
    cin >> ship_cmd_word;
    if (!ship_cmds.count(ship_cmd_word)) {
        throw Error("Unrecognized command!");
    }
    ship_cmds[ship_cmd_word](Model::get_instance().get_ship_ptr(ship_name));
}

void Controller::view_model_cmd(const string& cmd){
    if(view_model_cmds.count(cmd)){
        view_model_cmds[cmd]();
    } else {
        throw Error("Unrecognized command!");
    }
}

void Controller::clear(){
    for_each(all_views.begin(), all_views.end(), bind(&Model::detach,
                                                      &Model::get_instance(), _1));

    cout << "Done" << endl;
}

void Controller::view_cmd_default() const{
    if (!map_view){
        throw Error("Map view is not open!");
    }
    map_view->set_defaults();

}

void Controller::view_cmd_size() const{
    if (!map_view){
        throw Error("Map view is not open!");
    }
    int sz;
    if(!(cin >> sz)){
        throw Error("Expected an integer!");
    }
    map_view->set_size(sz);
}

void Controller::view_cmd_zoom() const{
    if (!map_view){
        throw Error("Map view is not open!");
    }
    map_view->set_scale(read_double());
}

void Controller::view_cmd_pan() const{
    if (!map_view){
        throw Error("Map view is not open!");
    }
    map_view->set_origin(read_point());
}

void Controller::view_cmd_show() const{
    for_each(all_views.begin(), all_views.end(), bind(&View::draw,_1));
}

void Controller::open_map_view(){
    if (map_view) {
        throw Error("Map view is already open!");
    }
    map_view = make_shared<Map_view>();
    all_views.push_back(map_view);
    Model::get_instance().attach(map_view);
}
void Controller::close_map_view(){
    if (!map_view) {
        throw Error("Map view is not open!");
    }
    close_view_helper(map_view);
}
void Controller::open_sailing_view(){
    if (sailing_data_view) {
        throw Error("Sailing data view is already open!");
    }
    sailing_data_view = make_shared<Sailing_data_view>();
    all_views.push_back(sailing_data_view);
    Model::get_instance().attach(sailing_data_view);

}
void Controller::close_sailing_view(){
    if (!sailing_data_view) {
        throw Error("Sailing data view is not open!");
    }
    close_view_helper(sailing_data_view);
}
void Controller::open_bridge_view(){
    auto ship_ptr = get_ship();
    if (bridge_views.count(ship_ptr->get_name())) {
        throw Error("Bridge view is already open for that ship!");
    }
    auto new_view = make_shared<Bridge_view>(ship_ptr->get_name());
    bridge_views[ship_ptr->get_name()] = new_view;
    all_views.push_back(new_view);
    Model::get_instance().attach(new_view);

}
void Controller::close_bridge_view(){
    string ship_name;
    cin >> ship_name;
    if (!bridge_views.count(ship_name)) {
        throw Error("Bridge view for that ship is not open!");
    }
    auto b_view_ptr = bridge_views[ship_name];
    bridge_views.erase(ship_name);
    close_view_helper(b_view_ptr);
}
void Controller::close_view_helper(std::shared_ptr<View>& sp){
    Model::get_instance().detach(sp);
    all_views.erase(find(all_views.begin(), all_views.end(), sp));
    sp.reset();
}
void Controller::model_cmd_status() const{
    Model::get_instance().describe();
}

void Controller::model_cmd_go() const{
    Model::get_instance().update();
}

void Controller::model_cmd_create() const{
    string name;
    cin >> name;
    if (name.length() < min_name_len) {
        throw Error("Name is too short!");
    }
    if (Model::get_instance().is_name_in_use(name)) {
        throw Error("Name is already in use!");
    }
    
    string type;
    cin >> type;
    Model::get_instance().add_ship(create_ship(name, type, read_point()));
    
}

void Controller::ship_course(shared_ptr<Ship> ship_ptr) const{
    double course = read_compass_heading();
    double speed = read_speed();
    ship_ptr->set_course_and_speed(course, speed);
}

void Controller::ship_position(shared_ptr<Ship> ship_ptr) const{
    Point destination = read_point();
    double speed = read_speed();
    ship_ptr->set_destination_position_and_speed(destination, speed);
}

void Controller::ship_destination(shared_ptr<Ship> ship_ptr) const{
    Point island_location = get_island()->get_location();
    double speed = read_speed();
    ship_ptr->set_destination_position_and_speed(island_location,
                                                 speed);
}

void Controller::ship_load_at(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->set_load_destination(get_island());
}

void Controller::ship_unload_at(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->set_unload_destination(get_island());
}

void Controller::ship_dock_at(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->dock(get_island());
}

void Controller::ship_attack(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->attack(get_ship());
}

void Controller::ship_refuel(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->refuel();
}

void Controller::ship_stop(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->stop();
}

void Controller::ship_stop_attack(shared_ptr<Ship> ship_ptr) const{
    ship_ptr->stop_attack();
}

shared_ptr<Ship> Controller::get_ship() const{
    string name;
    cin >> name;
    return Model::get_instance().get_ship_ptr(name);
}

shared_ptr<Island> Controller::get_island() const{
    string name;
    cin >> name;
    return Model::get_instance().get_island_ptr(name);
}

double Controller::read_double() const{
    double read_d;
    if(!(cin >> read_d)){
        throw Error("Expected a double!");
    }
    return read_d;
}

Point Controller::read_point() const{
    double point_x = read_double();
    double point_y = read_double();
    return Point{point_x, point_y};
}

double Controller::read_compass_heading() const{
    double compass_heading = read_double();
    if (compass_heading < 0. || compass_heading >= max_compass_heading) {
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

