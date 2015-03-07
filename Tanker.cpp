#include "Tanker.h"
#include "Island.h"

#include <iostream>
using namespace std;

Tanker::Tanker(const string& name_, Point position_):Ship{name_, position_, init_fuel_capacity_c, init_max_speed_c, init_fuel_consumption_c, init_resistance_c}{
    cout << "Tanker " << name_ << " constructed" << endl;
}

Tanker::~Tanker(){
    cout << "Tanker " << get_name() << " destructed" << endl;
}

void Tanker::set_destination_position_and_speed(Point destination,
                                                double speed){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    } else {
        Ship::set_destination_position_and_speed(destination, speed);
    }
}

void Tanker::set_course_and_speed(double course, double speed){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    } else {
        Ship::set_course_and_speed(course, speed);
    }
}

void Tanker::set_load_destination(Island* load_des_){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    } else if(load_des_ == unload_destination){
        throw Error("Load and unload cargo destinations are the same!");
    } else {
        cout << get_name() << " will load at " << load_des_->get_name() << endl;
    }
}

void Tanker::set_unload_destination(Island* unload_des_){
    if(tanker_state != State::NO_CARGO_DESTINATION){
        throw Error("Tanker has cargo destinations!");
    } else if(unload_des_ == load_destination){
        throw Error("Load and unload cargo destinations are the same!");
    } else {
        cout << get_name() << " will unload at " << unload_des_->get_name()
                          << endl;
    }
}

void Tanker::set_load_unload_helper(){
    if(is_docked() && get_docked_Island() == load_destination){
        tanker_state = State::LOADING;
    } else if(is_docked() && get_docked_Island() == unload_destination){
        tanker_state = State::UNLOADING;
    } else if(!is_moving() && cargo == 0 && can_dock(load_destination)){
        dock(load_destination);
        tanker_state = State::LOADING;
    } else if(!is_moving() && cargo > 0 && can_dock(unload_destination)){
        dock(unload_destination);
        tanker_state = State::UNLOADING;
    } else if(cargo == 0){
        Ship::set_destination_position_and_speed(
                    load_destination->get_location(), get_maximum_speed());
        tanker_state = State::MOVING_TO_LOADING;
    } else if(cargo > 0){
        Ship::set_destination_position_and_speed(
                                                 unload_destination->get_location(), get_maximum_speed());
        tanker_state = State::MOVING_TO_UNLOADING;
    }
}

void Tanker::stop(){
    Ship::stop();
    load_destination = nullptr;
    unload_destination = nullptr;
    tanker_state = State::NO_CARGO_DESTINATION;
    cout << get_name() <<  " now has no cargo destinations" << endl;
}

void Tanker::update(){
    Ship::update();
   if (!can_move() && tanker_state != State::NO_CARGO_DESTINATION) {
        tanker_state = State::NO_CARGO_DESTINATION;
        load_destination = nullptr;
        unload_destination = nullptr;
        cout << get_name() <<  " now has no cargo destinations" << endl;
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
    cout << "Cargo: " << cargo << " tons" << endl;
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
