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

const int distinct_name_len_c = 2;

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
    auto island_pair_ite = islands.lower_bound(name);
    if (island_pair_ite != islands.end() &&
        !island_pair_ite->second->get_name().compare(
            0,distinct_name_len_c, name, 0, distinct_name_len_c))
    {
        return true;
    }
    
    auto ship_pair_ite = ships.lower_bound(name);
    if (ship_pair_ite != ships.end() &&
        !ship_pair_ite->second->get_name().compare(
            0, distinct_name_len_c, name, 0 , distinct_name_len_c))
    {
        return true;
    }
    return false;
}

bool Model::is_island_present(const string& name) const{
    return islands.count(name);
}

shared_ptr<Island> Model::get_island_ptr(const string& name) const{
    auto ite = islands.find(name);
    if (ite == islands.end()) {
        throw Error("Island not found!");
    }
    return ite->second;
}

bool Model::is_ship_present(const string& name) const{
    return ships.count(name);
}

void Model::add_ship(shared_ptr<Ship> ship_ptr){
    ship_ptr->broadcast_current_state();
    add_ship_helper(ship_ptr);
}

shared_ptr<Ship> Model::get_ship_ptr(const string& name) const{
    auto ite = ships.find(name);
    if (ite == ships.end()) {
        throw Error("Ship not found!");
    }
    return ite->second;
}

void Model::describe() const{
    for_each(sim_objects.begin(), sim_objects.end(),
             mem_fn(&Sim_object::describe));
}

void Model::update(){
    time ++;
    for_each(sim_objects.begin(), sim_objects.end(),
             mem_fn(&Sim_object::update));
}

void Model::attach(shared_ptr<View> v){
    views.push_back(v);
    for_each(sim_objects.begin(), sim_objects.end(),
             mem_fn(&Sim_object::broadcast_current_state));
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
    sim_objects.erase(ship_ptr);
    ships.erase(ship_ptr->get_name());
}

void Model::add_island_helper(std::shared_ptr<Island> island_ptr){
    islands[island_ptr->get_name()] = island_ptr;
    sim_objects.insert(island_ptr);
}
void Model::add_ship_helper(std::shared_ptr<Ship> ship_ptr){
    ships[ship_ptr->get_name()] = ship_ptr;
    sim_objects.insert(ship_ptr);
}

vector<shared_ptr<Island>> Model::get_islands() const{
    vector<shared_ptr<Island>> islands_v;
    transform(islands.begin(), islands.end(),
      insert_iterator<vector<shared_ptr<Island>>>{islands_v, islands_v.begin()},
      mem_fn(&Islands_t::value_type::second));
    return islands_v;
}

// Compare Sim_object by name
bool Model:: Sim_object_name_comparator:: operator() (std::shared_ptr<Sim_object> lhs,
                                                      std::shared_ptr<Sim_object> rhs) {
    return lhs->get_name() < rhs->get_name();
}

