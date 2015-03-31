#include "Island.h"
#include "Model.h"

#include <iostream>

using namespace std;

Island::Island(const string& name_, Point position_, double fuel_,
               double production_rate_): Sim_object{name_}, position{position_},
               fuel{fuel_}, production_rate{production_rate_}{
    cout << "Island " << name_ << " constructed" << endl;
}

Island::~Island(){
    cout << "Island " << get_name() << " destructed" << endl;
}

double Island::provide_fuel(double request){
    double offer_fuel = fuel > request ? request : fuel;
    fuel -= offer_fuel;
    cout << "Island " << get_name() << " supplied " << offer_fuel <<
    " tons of fuel" << endl;
    return offer_fuel;
}

void Island::accept_fuel(double amount){
    fuel += amount;
    cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
}

void Island::update(){
    if (production_rate > 0){
        accept_fuel(production_rate);
    }
}

void Island::describe() const{
    cout << "\nIsland " << get_name() << " at position " << position << endl;
    cout << "Fuel available: " << fuel << " tons" << endl;
}

void Island::broadcast_current_state(){
    Model::get_instance().notify_location(get_name(), position);
}
