#include "Warship.h"
#include "Utility.h"

#include <iostream>
using namespace std;

Warship::Warship(const std::string& name_, Point position_, double fuel_capacity_, double maximum_speed_, double fuel_consumption_, int resistance_, int firepower_, double maximum_range_):
    Ship{name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_,
    resistance_}, firepower{firepower_},  maximum_range{maximum_range_}{
    cout << "Warship " << name_ << " constructed" << endl;
    
}

Warship::~Warship(){
    cout << "Warship " << get_name() << " destructed" << endl;
}

void Warship::update(){
    Ship::update();
    if (is_attacking() && (!is_afloat() || !target_ptr->is_afloat())) {
        stop_attack();
    } else if(is_attacking()){
        cout <<  get_name() << " is attacking" << endl;
    }
}

void Warship::attack(Ship* target_ptr_){
    if (!is_afloat()) {
        throw Error("Cannot attack!");
    }
    if (target_ptr_ == this) {
        throw Error("Warship may not attack itself!");
    }
    if (target_ptr_ == target_ptr) {
        throw Error("Already attacking this target!");
    }
    target_ptr = target_ptr_;
    is_attacking_state = true;
    cout << get_name() <<" will attack " << target_ptr->get_name() << endl;
}

void Warship::stop_attack(){
    if (!is_attacking()) {
        throw Error("Was not attacking!");
    }
    cout << get_name() << " stopping attack" << endl;
    target_ptr = nullptr;
    is_attacking_state = false;
}

void Warship::describe() const{
    Ship::describe();
    if (is_attacking()) {
        cout << "Attacking " << target_ptr->get_name() << endl;
    }
}

bool Warship::is_attacking() const{
    return is_attacking_state;
}

void Warship::fire_at_target(){
    cout << get_name() << " fires" << endl;
    target_ptr->receive_hit(firepower, this);
}

bool Warship::target_in_range() const{
    return cartesian_distance(get_location(), target_ptr->get_location())
           <= maximum_range;
}

Ship* Warship::get_target() const{
    return target_ptr;
}


