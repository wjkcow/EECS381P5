#include "Tanker.h"
#include "Island.h"
#include "Utility.h"

#include <iostream>
using namespace std;

// consts for initializations
const double init_fuel_capacity_c = 100.;
const double init_max_speed_c = 10.;
const double init_fuel_consumption_c = 2.;
const int init_resistance_c = 0;
const double init_cargo_capacity_c = 1000.;
const double init_cargo_c = 0.;
const double cargo_error_c = 0.05;

enum class Tanker::State{  // state enum for the tanker FSM
    NO_CARGO_DESTINATION,
    UNLOADING,
    MOVING_TO_LOADING,
    LOADING,
    MOVING_TO_UNLOADING
};

Tanker::Tanker(const string& name_, Point position_):Ship{name_, position_,
    init_fuel_capacity_c, init_max_speed_c, init_fuel_consumption_c,
    init_resistance_c}, cargo_capacity{init_cargo_capacity_c},cargo{init_cargo_c}, tanker_state{State::NO_CARGO_DESTINATION}{
}

// This class overrides these Ship functions so that it can check if this Tanker has assigned cargo destinations.
// if so, throw an Error("Tanker has cargo destinations!"); otherwise, simply call the Ship functions.
void Tanker::set_destination_position_and_speed(Point destination,
                                                double speed){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    }
    Ship::set_destination_position_and_speed(destination, speed);
}

void Tanker::set_course_and_speed(double course, double speed){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    }
    Ship::set_course_and_speed(course, speed);
}

// Set the loading and unloading Island destinations
// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
// if both destinations are now set, start the cargo cycle
void Tanker::set_load_destination(shared_ptr<Island> load_des_){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    }
    load_destination = load_des_;
    if(load_destination == unload_destination){
        throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will load at " << load_des_->get_name() << endl;
    set_load_unload_state();
}

void Tanker::set_unload_destination(shared_ptr<Island> unload_des_){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    }
    unload_destination = unload_des_;
    if(load_destination == unload_destination){
        throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will unload at " << unload_des_->get_name() << endl;
    set_load_unload_state();
}



void Tanker::stop(){
    Ship::stop();
    load_destination.reset();
    unload_destination.reset();
    tanker_state = State::NO_CARGO_DESTINATION;
    cout << get_name() <<  " now has no cargo destinations" << endl;
}

void Tanker::update(){
    Ship::update();
    if (!can_move()) {
        tanker_state = State::NO_CARGO_DESTINATION;
        load_destination.reset();
        unload_destination.reset();
        cout << get_name() <<  " now has no cargo destinations" << endl;
    } else if(tanker_state == State::NO_CARGO_DESTINATION){
        return;
    } else if (tanker_state == State::MOVING_TO_LOADING && !is_moving()
               && can_dock(load_destination)) {
        dock(load_destination);
        tanker_state = State::LOADING;
    } else if (tanker_state == State::MOVING_TO_UNLOADING && !is_moving()
               && can_dock(unload_destination)) {
        dock(unload_destination);
        tanker_state = State::UNLOADING;
    } else if (tanker_state == State::LOADING) {
        Ship::refuel();
        double cargo_needed = cargo_capacity - cargo;
        if (cargo_needed < cargo_error_c) {
            cargo = cargo_capacity;
            Ship::set_destination_position_and_speed(
                                                     unload_destination->get_location(), get_maximum_speed());
            tanker_state = State::MOVING_TO_UNLOADING;
        } else {
            cargo += load_destination->provide_fuel(cargo_needed);
            cout << get_name() << " now has " << cargo << " of cargo" << endl;
        }
    } else if (tanker_state == State::UNLOADING && cargo == 0.) {
        Ship::set_destination_position_and_speed
        (load_destination->get_location(), get_maximum_speed());
        tanker_state = State::MOVING_TO_LOADING;
    } else if (tanker_state == State::UNLOADING && cargo > 0.){
        unload_destination->accept_fuel(cargo);
        cargo = 0.;
    }
}

void Tanker::describe() const{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo << " tons";
    switch (tanker_state) {
        case State::NO_CARGO_DESTINATION:
            cout << ", no cargo destinations" << endl;
            break;
        case State::LOADING:
            cout << ", loading" << endl;
            break;
        case State::UNLOADING:
            cout << ", unloading" << endl;
            break;
        case State::MOVING_TO_LOADING:
            cout << ", moving to loading destination" << endl;
            break;
        case State::MOVING_TO_UNLOADING:
            cout << ", moving to unloading destination" << endl;
            break;
        default:
            break;
    }
}

// helper function for load and unload to change state
void Tanker::set_load_unload_state(){
    // change state only when both destination pointers are set
    if(!load_destination || !unload_destination){
        return;
    }
    if(is_docked() && get_docked_Island() == load_destination){
        tanker_state = State::LOADING;
    } else if(is_docked() && get_docked_Island() == unload_destination){
        tanker_state = State::UNLOADING;
    } else if(!is_moving() && cargo == 0. && can_dock(load_destination)){
        dock(load_destination);
        tanker_state = State::LOADING;
    } else if(!is_moving() && cargo > 0. && can_dock(unload_destination)){
        dock(unload_destination);
        tanker_state = State::UNLOADING;
    } else if(cargo == 0.){
        Ship::set_destination_position_and_speed(
                                                 load_destination->get_location(), get_maximum_speed());
        tanker_state = State::MOVING_TO_LOADING;
    } else if(cargo > 0.){
        Ship::set_destination_position_and_speed(
                                                 unload_destination->get_location(),
                                                 get_maximum_speed());
        tanker_state = State::MOVING_TO_UNLOADING;
    }
}
