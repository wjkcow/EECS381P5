#include "Ship.h"
#include "Model.h"

#include <iostream>

using namespace std;

Ship::Ship(const string& name_, Point position_, double fuel_capacity_,
           double maximum_speed_, double fuel_consumption_, int resistance_): Sim_object{name_}, Track_base(position_), fuel{fuel_capacity_},
maximum_speed(maximum_speed_),
fuel_consumption{fuel_consumption_}, resistance(resistance_)
{
    cout << "Ship " << name_ <<  " constructed" << endl;
    
}
/*
 Define the destructor function even if it was declared as a pure virtual function.
 This seems odd, because pure virtual functions are usually not defined in the class
 that declares them. But this is often done as a way to make a class abstract,
 if there is no other virtual function that makes sense to mark as pure. Here we
 are defining it just to get the destructor message output.
 */

Ship::~Ship()
{
    cout << "Ship "  << get_name() << " destructed" << endl;
}

bool Ship::can_move() const{
    return (ship_state != State::DEAD_IN_THE_WATER) &&
    (ship_state != State::SINKING) && (ship_state != State::SUNK) &&
    (ship_state != State::ON_THE_BOTTOM);
}

bool Ship::is_moving() const{
    return (ship_state == State::MOVING_TO_POSITION) ||
    (ship_state == State::MOVING_ON_COURSE);
}

bool Ship::is_docked() const{
    return ship_state == State::DOCKED;
}

bool Ship::is_afloat() const{
    return (ship_state != State::SINKING) && (ship_state != State::SUNK) &&
    (ship_state != State::ON_THE_BOTTOM);
}

bool Ship::is_on_the_bottom() const{
    return ship_state == State::ON_THE_BOTTOM;
}

bool Ship::can_dock(Island* island_ptr) const{
    if ((ship_state == State::STOPPED) &&
        (cartesian_distance(destination, Track_base::get_position())
         < dock_dist_c)) {
            return true;
        }
    return false;
}

void Ship::update(){
    if (is_afloat() && resistance >= 0 && is_moving()){ // we are not sinking!
        calculate_movement();
        cout << get_name() << " now at " << get_position() << endl;
    } else if(is_afloat() && resistance >= 0 && ship_state == State::STOPPED){
        
    } //else if(is_afloat() && resistance >= 0 && is_docked()){
        
    } else if(is_afloat() && resistance >= 0 && !can_move()){
        
    } else if (is_afloat() && resistance < 0){
        ship_state = State::SINKING;
        set_speed(0.);
        cout << get_name() << " sinking" << endl;
    } else if (ship_state == State::SINKING){
        ship_state = State::SUNK;
        g_Model_ptr->notify_gone(get_name());
        cout << get_name() <<" sunk" << endl;
    } else if (ship_state == State::SUNK){
        ship_state = State::ON_THE_BOTTOM;
        cout << get_name() <<" on the bottom" << endl;
    } else if (ship_state == State::ON_THE_BOTTOM){
        cout << get_name() <<" on the bottom" << endl;
    }
    
}

void Ship::describe() const{
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    
    cout << get_name() << " at " << get_position();
    switch (ship_state) {
        case State::SINKING:
            cout << " sinking" << endl;
            return;
            break;
        case State::SUNK:
            cout << " sunk" << endl;
            return;
            break;
        case State::ON_THE_BOTTOM:
            cout << " on the bottom" << endl;
            return;
            break;
        default:
            break;
    }
    cout << ", fuel: " << fuel << " tons, resistance: " << resistance << endl;
    switch (ship_state) {
        case State::MOVING_TO_POSITION:
            cout << "Moving to " << destination << " on " << get_course_speed()
            << endl;
            break;
        case State::MOVING_ON_COURSE:
            cout << "Moving on " << get_course_speed() << endl;
            break;
        case State::DOCKED:
            cout << "Docked at " << get_position() << endl;
            break;
        case State::STOPPED:
            cout << "Stopped" << endl;
            break;
        case State::DEAD_IN_THE_WATER:
            cout << "Dead in the water" << endl;
            break;
        default:
            break;
    }
    
}

void Ship::broadcast_current_state(){
    g_Model_ptr->notify_location(get_name(), get_position());
}

void Ship::set_destination_position_and_speed(Point destination_position, double speed){
    
    // get the course
    Compass_vector cv{get_position(), destination};
    move_helper(cv.direction, speed);
    // undock the ship
    if (ship_state == State::DOCKED) {
        ship_state = State::MOVING_TO_POSITION;
    }
    cout <<  get_name() <<" will sail on "  << get_course_speed() << " to " <<
    destination << endl;
}

void Ship::set_course_and_speed(double course, double speed){
    move_helper(course, speed);
    if (ship_state == State::DOCKED) {
        ship_state = State::MOVING_ON_COURSE;
    }
    cout << get_name() <<" will sail on " << get_course_speed() << endl;
}

void Ship::move_helper(double course, double speed){
    if (!can_move()) {
        throw Error("Ship cannot move!");
    }
    if (speed > maximum_speed) {
        throw Error("Ship cannot go that fast!");
    }
    Track_base::set_course(course);
    Track_base::set_speed(speed);
}

void Ship::stop(){
    if (!can_move()) {
        throw Error("Ship cannot move!");
    }
    Track_base::set_speed(0.);
    cout << get_name() <<  " stopping at " << get_position() << endl;
    ship_state = State::STOPPED;
}

void Ship::dock(Island * island_ptr){
    if (!can_dock(island_ptr)) {
        throw Error("Can't dock!");
    }
    set_position(island_ptr->get_location());
    g_Model_ptr->notify_location(get_name(), get_position());
    ship_state = State::DOCKED;
    docked_island = island_ptr;
    cout << get_name() << " docked at " << island_ptr->get_name() << endl;
}

void Ship::refuel(){
    if (!is_docked()){
        throw Error("Must be docked!");
    }
    double fuel_needed = fuel_capacity - fuel;
    if (fuel_needed < fuel_error_c){
        fuel = fuel_capacity;
    } else {
        fuel += docked_island->provide_fuel(fuel_needed);
        cout << get_name() <<  " now has " << fuel << " tons of fuel" << endl;
    }
    
}

void Ship::set_load_destination(Island *){
    throw  Error("Cannot load at a destination!");
}

void Ship::set_unload_destination(Island *){
    throw Error("Cannot unload at a destination!");
}

void Ship::attack(Ship * in_target_ptr){
    throw Error("Cannot attack!");
}

void Ship::stop_attack(){
    throw Error("Cannot attack!");
}

void Ship::receive_hit(int hit_force, Ship* attacker_ptr){
    resistance -= hit_force;
    cout << get_name() <<  " hit with " << hit_force
    <<  ", resistance now " << resistance << endl;
}

/* Private Function Definitions */

double Ship::get_maximum_speed() const{
    return maximum_speed;
}

Island* Ship::get_docked_Island() const{
    return docked_island;
}

/*
 Calculate the new position of a ship based on how it is moving, its speed, and
 fuel state. This function should be called only if the state is
 MOVING_TO_POSITION or MOVING_ON_COURSE.
 
 Track_base has an update_position(double time) function that computes the new position
 of an object after the specified time has elapsed. If the Ship is going to move
 for a full time unit (one hour), then it will get go the "full step" distance,
 so update_position would be called with time = 1.0. If we can move less than that,
 e.g. due to not enough fuel, update position  will be called with the corresponding
 time less than 1.0.
 
 For clarity in specifying the computation, this code assumes the specified private variable names,
 but you may change the variable names or enum class names, or state names if you wish (e.g. movement_state).
 */
void Ship:: calculate_movement()
{
    // Compute values for how much we need to move, and how much we can, and how long we can,
    // given the fuel state, then decide what to do.
    double time = 1.0;	// "full step" time
    // get the distance to destination
    double destination_distance = cartesian_distance(get_location(), destination);
    // get full step distance we can move on this time step
    double full_distance = get_speed() * time;
    // get fuel required for full step distance
    double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
    // how far and how long can we sail in this time period based on the fuel state?
    double distance_possible, time_possible;
    if(full_fuel_required <= fuel) {
        distance_possible = full_distance;
        time_possible = time;
    }
    else {
        distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
        time_possible = (distance_possible / full_distance) * time;
    }
    
    // are we are moving to a destination, and is the destination within the distance possible?
    if(ship_state == State::MOVING_TO_POSITION && destination_distance <= distance_possible) {
        // yes, make our new position the destination
        set_position(destination);
        // we travel the destination distance, using that much fuel
        double fuel_required = destination_distance * fuel_consumption;
        fuel -= fuel_required;
        set_speed(0.);
        ship_state = State::STOPPED;
    }
    else {
        // go as far as we can, stay in the same movement state
        // simply move for the amount of time possible
        update_position(time_possible);
        // have we used up our fuel?
        if(full_fuel_required >= fuel) {
            fuel = 0.0;
            set_speed(0.);
            ship_state = State::DEAD_IN_THE_WATER;
        }
        else {
            fuel -= full_fuel_required;
        }
    }
}

