#include "Cruise_ship.h"
#include "Geometry.h"
#include "Island.h"

#include "Model.h"
#include <algorithm>
#include <iostream>
using namespace std;

Cruise_ship::Cruise_ship(const string& name_, Point position_):
Ship{name_, position_,
    init_fuel_capacity_c, init_max_speed_c, init_fuel_consumption_c,
    init_resistance_c}{

}

void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed){
    cancel_if_on_cruise();
    
    first_island = get_first_island(destination_position);
    if (first_island) {
        cruise_helper(destination_position, speed);
        state = State::MOVING_TO_NEXT_ISLAND;
        next_destination = first_island;
    } else {
        Ship::set_destination_position_and_speed(destination_position, speed);
    }
    
}

void Cruise_ship::cruise_helper(Point destination_position, double speed){
    Ship::set_destination_position_and_speed(destination_position, speed);
    cout << get_name() << " will visit " << first_island->get_name() << endl;

}

void Cruise_ship::set_course_and_speed(double course, double speed){
    cancel_if_on_cruise();
    Ship::set_course_and_speed(course, speed);
}

void Cruise_ship::stop(){
    cancel_if_on_cruise();
    Ship::stop();
}

void Cruise_ship::dock(shared_ptr<Island> island_ptr){
    cancel_if_on_cruise();
    Ship::dock(island_ptr);
}

void Cruise_ship::update(){
    Ship::update();
    if (!can_move()) {
        cancel_if_on_cruise();
    }
    switch (state) {
        case State::NOT_ON_CRUISE:
            break;
        case State::REFUELING:
            Ship::refuel();
            break;
        case State::VISITING_ISLAND:
            state = State::MOVING_TO_NEXT_ISLAND;
            break;
        case State::MOVING_TO_NEXT_ISLAND:
            if (can_dock(next_destination)) {
                dock(next_destination);
                state = State::REFUELING;
                visited_islands.insert(next_destination);
                next_destination = get_next_island();
            }
            if (get_docked_Island() == first_island) {
                state = State::NOT_ON_CRUISE;
            }
            break;
        default:
            break;
    }
}

void Cruise_ship::describe() const{
    cout << "\nCruise_ship ";
    Ship::describe();
    if (state == State::VISITING_ISLAND) {
        cout << "Waiting during cruise at "
             << get_docked_Island()->get_name() << endl;

    } else if(state == State::MOVING_TO_NEXT_ISLAND) {
        cout << "On cruise to " << next_destination->get_name() << endl;
    }
}

void Cruise_ship::cancel_if_on_cruise(){
    if (!is_on_cruise()) {
        cout << get_name() << " canceling current cruise" << endl;
        state = State::NOT_ON_CRUISE;
        visited_islands.clear();
    }
}

shared_ptr<Island> Cruise_ship::get_first_island(Point destination){
    const Model::Islands_t &islands = Model::get_instance().get_islands();
    auto ite = find_if(islands.begin(), islands.end(), [destination](Model::Islands_t::value_type name_island_pair){
        return name_island_pair.second->get_location() == destination;
    });
    if (ite == islands.end()) {
        return shared_ptr<Island>{};
    }
    first_island = ite->second;
    return first_island;
}

shared_ptr<Island> Cruise_ship::get_next_island(){
    shared_ptr<Island> next_island{};
    const Model::Islands_t &islands = Model::get_instance().get_islands();
    for (auto &island_pair: islands) {
        bool not_visited = !visited_islands.count(island_pair.second);
        if (!not_visited && get_next_island_helper(next_island, island_pair.second)) {
            next_island = island_pair.second;
        }
    }
    return next_island;
}

// return true if the candiate is choosen, otherwise false
bool Cruise_ship::get_next_island_helper(std::shared_ptr<Island> last_best,
                                         std::shared_ptr<Island> candidate){
    if (!last_best ||
        cartesian_distance(next_destination->get_location(), last_best->get_location())
        >  cartesian_distance(next_destination->get_location(), candidate->get_location())) {
        return true;
    } else {
        return false;
    }
}

