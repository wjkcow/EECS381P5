#include "Cruiser.h"
#include <iostream>

using namespace std;

Cruiser::Cruiser(const std::string& name_, Point position_):
    Warship{name_, position_, fuel_capacity_c, maximum_speed_c, fuel_consumption_c, resistance_c, firepower_c, maximum_attacking_range_}{
}

void Cruiser::update(){
    Warship::update();
    if(is_attacking() && target_in_range()){
        fire_at_target();
    } else if(is_attacking() && !target_in_range()){
        cout << get_name() << " target is out of range" << endl;
        stop_attack();
    }
}

void Cruiser::describe() const{
    cout << "\nCruiser ";
    Warship::describe();
}

void Cruiser::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr){
    Ship::receive_hit(hit_force, attacker_ptr);
    if (is_afloat() && !is_attacking()) {
        Warship::attack(attacker_ptr);
    }
}
