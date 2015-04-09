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
#include <functional>
#include <memory>
using namespace std;

Model& Model::get_instance(){
    static Model instance{};
    return instance;
}

Model::Model(){
    add_island_helper(make_shared<Island>("Exxon", Point(10, 10), 1000, 200));
    add_island_helper(make_shared<Island>("Shell", Point(0, 30), 1000, 200));
    add_island_helper(make_shared<Island>("Bermuda", Point(20, 20)));
    add_island_helper(make_shared<Island>("Treasure_Island",
                                          Point(50, 5), 100, 5));
    
    add_ship_helper(create_ship("Ajax", "Cruiser", Point (15, 15)));
    add_ship_helper(create_ship("Xerxes", "Cruiser", Point (25, 25)));
    add_ship_helper(create_ship("Valdez", "Tanker", Point (30, 30)));
}

Model::~Model(){}

bool Model::is_name_in_use(const string& name) const{
    return sim_objects.count(name); //NOTE: search map containers
    // is_island_present
    // is_is_ship_present
}

bool Model::is_island_present(const string& name) const{
    return islands.count(name); //NOTE: comp: bi
}

shared_ptr<Island> Model::get_island_ptr(const string& name) const{
    if(!is_island_present(name)){ //NOTE: double search
        throw Error("Island not found!");
    }
    return islands.find(name)->second; //NOTE: at
}

bool Model::is_ship_present(const string& name) const{
    return ships.count(name);
}

void Model::add_ship(shared_ptr<Ship> ship_ptr){
    ship_ptr->broadcast_current_state();
    add_ship_helper(ship_ptr);
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
    time ++; //NOTE: ++
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

// notify the change of fuel
void Model::notify_fuel(const std::string& name, double fuel){
    for_each(views.begin(), views.end(),
             bind(&View::update_ship_fuel,std::placeholders::_1,name, fuel));
}

// notify the change of course
void Model::notify_course(const std::string& name, double course){
    for_each(views.begin(), views.end(),
             bind(&View::update_ship_course,std::placeholders::_1,name, course));
}

// notify the change of speed
void Model::notify_speed(const std::string& name, double speed){
    for_each(views.begin(), views.end(),
             bind(&View::update_ship_speed,std::placeholders::_1,name, speed));
}


void Model::notify_gone(const string& name){
    for_each(views.begin(), views.end(),
             bind(&View::update_remove,std::placeholders::_1,name));
}

void Model::remove_ship(shared_ptr<Ship> ship_ptr){
    sim_objects.erase(ship_ptr->get_name());
    ships.erase(ship_ptr->get_name());
}

void Model::add_island_helper(std::shared_ptr<Island> island_ptr){
    islands[island_ptr->get_name()] = island_ptr;
    sim_objects[island_ptr->get_name()] = island_ptr;
}
void Model::add_ship_helper(std::shared_ptr<Ship> ship_ptr){
    ships[ship_ptr->get_name()] = ship_ptr;
    sim_objects[ship_ptr->get_name()] = ship_ptr;
}

vector<shared_ptr<Island>> Model::get_islands(){
    vector<shared_ptr<Island>> islands_v;
    transform(islands.begin(), islands.end(),
      insert_iterator<vector<shared_ptr<Island>>>{islands_v, islands_v.begin()},
      bind(&Islands_t::value_type::second, placeholders::_1));
    return islands_v;
}

