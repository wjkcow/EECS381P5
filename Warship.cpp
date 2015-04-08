#include "Warship.h"
#include "Utility.h"

#include <iostream>
using namespace std;

Warship::Warship(const string& name_, Point position_, double fuel_capacity_, double maximum_speed_, double fuel_consumption_, int resistance_, int firepower_, double maximum_range_):
    Ship{name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_,
    resistance_}, firepower{firepower_},  maximum_range{maximum_range_}
{
}

void Warship::update(){
    Ship::update();
    //NOTE: not is_attacking return
    if (is_attacking() && (!is_afloat() || target_ptr.expired() ||
                           !get_target()->is_afloat())) {
        stop_attack();
        target_ptr.reset(); //NOTE: stop_attack
    } else if(is_attacking()){
        cout <<  get_name() << " is attacking" << endl;
    }
}

void Warship::attack(shared_ptr<Ship> target_ptr_){
    if (!is_afloat()) {
        throw Error("Cannot attack!");
    }
    if (target_ptr_ == shared_from_this()) {
        throw Error("Warship may not attack itself!");
    }
    if (target_ptr_ == get_target()) {
        throw Error("Already attacking this target!");
    }
    target_ptr = target_ptr_;
    is_attacking_state = true;
    cout << get_name() <<" will attack " << get_target()->get_name() << endl;
}

void Warship::stop_attack(){
    if (!is_attacking()) {
        throw Error("Was not attacking!");
    }
    cout << get_name() << " stopping attack" << endl;
    target_ptr.reset();
    is_attacking_state = false;
}

void Warship::describe() const{
    Ship::describe(); //NOTE: !is_attacking()
    if (is_attacking() && (target_ptr.expired() || !get_target()->is_afloat())){
        cout << "Attacking absent ship" << endl;
    } else if (is_attacking()) {
        cout << "Attacking " << get_target()->get_name() << endl;
    }
}

bool Warship::is_attacking() const{
    return is_attacking_state;
}

void Warship::fire_at_target(){
    cout << get_name() << " fires" << endl;
    get_target()->receive_hit(firepower, shared_from_this());
}

bool Warship::target_in_range() const{
    return cartesian_distance(get_location(), get_target()->get_location())
           <= maximum_range;
}

shared_ptr<Ship> Warship::get_target() const{
    return target_ptr.lock();
}


