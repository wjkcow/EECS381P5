#include "Cruise_ship.h"
#include "Geometry.h"
#include "Island.h"

#include "Model.h"
#include <algorithm>
#include <iostream>
using namespace std;

const double init_fuel_capacity_c = 500.;
const double init_max_speed_c = 15.;
const double init_fuel_consumption_c = 2.;
const int    init_resistance_c = 0;

enum class Cruise_ship::State{
    NOT_ON_CRUISE,
    REFUELING,          // day 1
    VISITING_ISLAND,    // day 2
    CONTINUE_CRUISE,    // day 3
    MOVING_TO_NEXT_ISLAND
};

Cruise_ship::Cruise_ship(const string& name_, Point position_):
Ship{name_, position_,
    init_fuel_capacity_c, init_max_speed_c, init_fuel_consumption_c,
    init_resistance_c},cruise_ship_state{State::NOT_ON_CRUISE}{
}

// set destination position and speed to the ship, if it is the position of
// a island, the cruise_ship will start the cruise from the island, if it is
// already on cruise, the cruise will be canceled
void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed){
    cancel_if_on_cruise();
    Ship::set_destination_position_and_speed(destination_position, speed);
    first_island = get_island_at_location(destination_position);
    if (first_island) {
        next_destination = first_island;
        cruise_speed = speed;
        cout << get_name() << " will visit " << next_destination->get_name() << endl;
        cout <<  get_name() << " cruise will start and end at "
             << first_island->get_name() << endl;
        cruise_ship_state = State::MOVING_TO_NEXT_ISLAND;
    }
}

// cruise will be canceled,
void Cruise_ship::set_course_and_speed(double course, double speed){
    cancel_if_on_cruise();
    Ship::set_course_and_speed(course, speed);
}

// cruise will be canceled
void Cruise_ship::stop(){
    cancel_if_on_cruise();
    Ship::stop();
}



// move like a ship or cruise
void Cruise_ship::update(){
    Ship::update();
    if (!can_move()) {
        cancel_if_on_cruise();
    }
    switch (cruise_ship_state) {
        case State::NOT_ON_CRUISE:
            break;
        case State::REFUELING:
            Ship::refuel();
            cruise_ship_state = State::VISITING_ISLAND;
            break;
        case State::VISITING_ISLAND:
            cruise_ship_state = State::CONTINUE_CRUISE;
            break;
        case State::CONTINUE_CRUISE:
            cruise_ship_state = State::MOVING_TO_NEXT_ISLAND;
            next_destination = get_next_island_and_remove();
            Ship::set_destination_position_and_speed(
                                             next_destination->get_location(),
                                             cruise_speed);
            cout << get_name() << " will visit " << next_destination->get_name() << endl;
            break;
        case State::MOVING_TO_NEXT_ISLAND:
            // if we dock at the first island and have visited all the islands,
            //the crusie is done, otherwise we just dock at next island and
            // start refuelling
            if (can_dock(first_island) &&
                unvisited_islands.empty()) {
                Ship::dock(first_island);
                cruise_ship_state = State::NOT_ON_CRUISE;
                cout << get_name() <<  " cruise is over at " << first_island->get_name() << endl;
            } else if (can_dock(next_destination)) {
                Ship::dock(next_destination);
                cruise_ship_state = State::REFUELING;
            }
            break;
        default:
            break;
    }
}

void Cruise_ship::describe() const{
    cout << "\nCruise_ship ";
    Ship::describe();
    if (cruise_ship_state == State::VISITING_ISLAND
        || cruise_ship_state == State::REFUELING
        || cruise_ship_state == State::CONTINUE_CRUISE) {
        cout << "Waiting during cruise at "
             << get_docked_Island()->get_name() << endl;
    } else if(cruise_ship_state == State::MOVING_TO_NEXT_ISLAND) {
        cout << "On cruise to " << next_destination->get_name() << endl;
    }
}

void Cruise_ship::cancel_if_on_cruise(){
    if (cruise_ship_state != State::NOT_ON_CRUISE) {
        cout << get_name() << " canceling current cruise" << endl;
        cruise_ship_state = State::NOT_ON_CRUISE;
        unvisited_islands.clear();
        first_island.reset();
        next_destination.reset();
    }
}

// get the island pointer from the destination Point,
// if no island at that destination, empty pointer is returned.
shared_ptr<Island> Cruise_ship::get_island_at_location(Point destination){
    // start the cruise, thus all island have no been visited
    unvisited_islands = Model::get_instance().get_islands();
    auto ite = find_if(unvisited_islands.begin(), unvisited_islands.end(),
                       [destination](const shared_ptr<Island>& island_ptr){
        return island_ptr->get_location() == destination;
    });
    if (ite == unvisited_islands.end()) {
        unvisited_islands.clear();
        return shared_ptr<Island>{};
    }
    first_island = *ite;
    unvisited_islands.erase(ite);
    return first_island;
}

// get the next island to cruise to depend on next_island and remove the island from unvisited islands
shared_ptr<Island> Cruise_ship::get_next_island_and_remove(){
    if (unvisited_islands.empty()) {
        return first_island;
    }
    auto ite = min_element(unvisited_islands.begin(), unvisited_islands.end(), [this]
            (const shared_ptr<Island> &l_sp,const shared_ptr<Island>& r_sp){
                return  cartesian_distance(next_destination->get_location(),
                                           l_sp->get_location())
                <  cartesian_distance(next_destination->get_location(),
                                      r_sp->get_location());
            });
    auto ret = *ite;
    // best place to erase cause no more search needed
    unvisited_islands.erase(ite);
    return ret;
}



