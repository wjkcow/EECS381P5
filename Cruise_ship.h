#ifndef CRUISE_SHIP
#define CRUISE_SHIP

#include "Ship.h"

#include <vector>
#include <memory>
class Island;

class Cruise_ship: public Ship{
public:
    Cruise_ship(const std::string& name_, Point position_);
    // set destination position and speed to the ship, if it is the position of
    // a island, the cruise_ship will start the cruise from the island, if it is
    // already on cruise, the cruise will be canceled
    void  set_destination_position_and_speed(Point destination_position, double speed) override;
    // cruise will be canceled,
    void set_course_and_speed(double course, double speed) override;
    // cruise will be canceled
    void stop();
    // cruise will be canceled
    void dock(std::shared_ptr<Island> island_ptr);
    // move like a ship or cruise
    void update() override;
    void describe() const override;
private:
    double cruise_speed; // cruise is always on this speed
    enum class State{
        NOT_ON_CRUISE,
        REFUELING,          // day 1
        VISITING_ISLAND,    // day 2
        CONTINUE_CRUISE,    // day 3
        MOVING_TO_NEXT_ISLAND
    };
    State state{State::NOT_ON_CRUISE};
    
    bool is_on_cruise() const{
        return state != State::NOT_ON_CRUISE;
    }
    // start the cruise
    void cruise_helper(Point destination_position);
    void cancel_if_on_cruise();
    // get the island pointer from the destination Point,
    // if no island at that destination, empty pointer is returned.
    std::shared_ptr<Island> get_first_island(Point destination);
    // get the next island to cruise to depend on next_island
    std::shared_ptr<Island> get_next_island();

    // next island we will visit
    std::shared_ptr<Island> next_destination;
    // first island to cruise to
    std::shared_ptr<Island> first_island;

    // all the island we have no visited
    std::vector<std::shared_ptr<Island>> unvisited_islands;

    constexpr static double init_fuel_capacity_c = 500.;
    constexpr static double init_max_speed_c = 15.;
    constexpr static double init_fuel_consumption_c = 2.;
    constexpr static int init_resistance_c = 0;
};

#endif
