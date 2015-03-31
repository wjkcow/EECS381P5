#include "Model.h"

#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Ship_factory.h"
#include "Utility.h"

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


Model& Model::get_instance(){
    static Model instance{};
    return instance;
}

Model::Model(){
    islands["Exxon"] = make_shared<Island>("Exxon", Point(10, 10), 1000, 200);
    islands["Shell"] = make_shared<Island>("Shell", Point(0, 30), 1000, 200);
    islands["Bermuda"] = make_shared<Island>("Bermuda", Point(20, 20));
    islands["Treasure_Island"] = make_shared<Island>("Treasure_Island",
                                                     Point(50, 5), 100, 5);
    
    ships["Ajax"] = create_ship("Ajax", "Cruiser", Point (15, 15));
    ships["Xerxes"] = create_ship("Xerxes", "Cruiser", Point (25, 25));
    ships["Valdez"] = create_ship("Valdez", "Tanker", Point (30, 30));
    
    sim_objects["Exxon"] = islands["Exxon"];
    sim_objects["Shell"] = islands["Shell"];
    sim_objects["Bermuda"] = islands["Bermuda"];
    
    sim_objects["Ajax"] = ships["Ajax"];
    sim_objects["Xerxes"] = ships["Xerxes"];
    sim_objects["Valdez"] = ships["Valdez"];
    cout << "Model constructed" << endl;
}

Model::~Model(){
    cout << "Model destructed" << endl;
}

bool Model::is_name_in_use(const string& name) const{
    return sim_objects.count(name);
}

bool Model::is_island_present(const string& name) const{
    return islands.count(name);
}

shared_ptr<Island> Model::get_island_ptr(const string& name) const{
    if(!is_island_present(name)){
        throw Error("Island not found!");
    }
    return islands.find(name)->second;
}

bool Model::is_ship_present(const string& name) const{
    return ships.count(name);
}

void Model::add_ship(shared_ptr<Ship> ship_ptr){
    ship_ptr->broadcast_current_state();
    ships[ship_ptr->get_name()] = ship_ptr;
    sim_objects[ship_ptr->get_name()] = ship_ptr;
}

shared_ptr<Ship> Model::get_ship_ptr(const string& name) const{
    if (!is_ship_present(name)) {
        throw Error("Ship not found!");
    }
    return ships.find(name)->second;
}

void Model::describe() const{
    for (auto &objects_pair : sim_objects) {
        objects_pair.second->describe();
    }
}

void Model::update(){
    time ++;
    for (auto &objects_pair : sim_objects) {
        objects_pair.second->update();
    }
}

void Model::attach(shared_ptr<View> v){
    views.push_back(v);
    for (auto so_pair : sim_objects) {
        so_pair.second->broadcast_current_state();
    }
}

void Model::detach(shared_ptr<View> v){
    views.erase(find(views.begin(), views.end(), v));
}

void Model::notify_location(const string& name, Point location){
    for_each(views.begin(), views.end(),
             bind(&View::update_location,std::placeholders::_1,name, location));
}

void Model::notify_sailing_data(const std::string& name, double fuel, double course, double speed){
    for_each(views.begin(), views.end(),
             bind(&View::update_sailing_data,std::placeholders::_1,name, fuel, course, speed));
}

void Model::notify_gone(const string& name){
    for_each(views.begin(), views.end(),
             bind(&View::update_remove,std::placeholders::_1,name));
}

void Model::remove_ship(shared_ptr<Ship> ship_ptr){
    sim_objects.erase(ship_ptr->get_name());
    ships.erase(ship_ptr->get_name());
}



