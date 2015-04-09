#include "Island.h"
#include "Model.h"

#include <iostream>

using namespace std;

Island::Island(const string& name_, Point position_, double fuel_,
               double production_rate_): Sim_object{name_}, position{position_},
               fuel{fuel_}, production_rate{production_rate_}{
}

// Return whichever is less, the request or the amount left,
// update the amount on hand accordingly, and output the amount supplied.
double Island::provide_fuel(double request){
    double offer_fuel = fuel > request ? request : fuel;
    fuel -= offer_fuel;
    cout << "Island " << get_name() << " supplied " << offer_fuel <<
    " tons of fuel" << endl;
    return offer_fuel;
}

// Add the amount to the amount on hand, and output the total as the amount the Island now has.
void Island::accept_fuel(double amount){
    fuel += amount;
    cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
}

// if production_rate > 0, compute production_rate * unit time, and add to amount, and print an update message
void Island::update(){
    if (production_rate > 0){
        accept_fuel(production_rate);
    }
}

// output information about the current state
void Island::describe() const{
    cout << "\nIsland " << get_name() << " at position " << position << endl;
    cout << "Fuel available: " << fuel << " tons" << endl;
}

// ask model to notify views of current state
void Island::broadcast_current_state(){
    Model::get_instance().notify_location(get_name(), position);
}
