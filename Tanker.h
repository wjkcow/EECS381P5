#ifndef TANKER_H
#define TANKER_H
#include "Ship.h"

class Island;
/*
A Tanker is a ship with a large corgo capacity for fuel.
It can be told an Island to load fuel at, and an Island to unload at.
Once it is sent to the loading destination, it will start shuttling between 
the loading and unloading destination. At the loading destination, 
it will first refuel then wait until its cargo hold is full, then it will
go to the unloading destination.

Initial values:
fuel capacity and initial amount 100 tons, maximum speed 10., fuel consumption 2.tons/nm, 
resistance 0, cargo capacity 1000 tons, initial cargo is 0 tons.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/
class Tanker: public Ship{
public:
	// initialize, the output constructor message
	Tanker(const std::string& name_, Point position_);
	// output destructor message
	~Tanker();
	
	// This class overrides these Ship functions so that it can check if this Tanker has assigned cargo destinations.
	// if so, throw an Error("Tanker has cargo destinations!"); otherwise, simply call the Ship functions.
    void set_destination_position_and_speed(Point destination, double speed) override;
	void set_course_and_speed(double course, double speed) override;

	// Set the loading and unloading Island destinations
	// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
	// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
	// if both destinations are now set, start the cargo cycle
	void set_load_destination(Island*) override;
	void set_unload_destination(Island*) override;
	
	// when told to stop, clear the cargo destinations and stop
	void stop() override;
	
	void update() override;
    void describe() const override;
private:
    double
    cargo_capacity = init_cargo_capacity_c; // capacity of the cargo
    double cargo = init_cargo_c; // current amount
    Island* load_destination = nullptr;
    Island* unload_destination = nullptr;
    
    // throw error if it's not NO_CARGO_DESTINATION
    void check_no_cargo_destination();
    // throw error is load and unload des are the same
    void check_destination();
    // helper function for load and unload to change state
    void set_load_unload_helper();
    enum class State{  // state enum for the tanker FSM
        NO_CARGO_DESTINATION,
        UNLOADING,
        MOVING_TO_LOADING,
        LOADING,
        MOVING_TO_UNLOADING
    };
    State tanker_state; // current state of the tanker
    // consts for initializations
    constexpr static double init_fuel_capacity_c = 100.;
    constexpr static double init_max_speed_c = 10.;
    constexpr static double init_fuel_consumption_c = 2.;
    constexpr static int init_resistance_c = 0;
    constexpr static double init_cargo_capacity_c = 1000.;
    constexpr static double init_cargo_c = 0.;
    constexpr static double cargo_error_c = 0.05;
};
#endif
