#ifndef CRUISE_SHIP
#define CRUISE_SHIP

#include "Ship.h"

#include <vector>
#include <memory>
class Island;

class Cruise_ship: public Ship{
public:
    Cruise_ship(const std::string& name_, Point position_);
    void  set_destination_position_and_speed(Point destination_position, double speed) override;
    void set_course_and_speed(double course, double speed) override;
    void stop();
    void dock(std::shared_ptr<Island> island_ptr);
    
    void update() override;
    void describe() const override;
private:
    double cruise_speed;
    enum class State{
        NOT_ON_CRUISE,
        REFUELING,
        VISITING_ISLAND,
        CONTINUE_CRUISE,
        MOVING_TO_NEXT_ISLAND
    };
    State state{State::NOT_ON_CRUISE};
    
    bool is_on_cruise() const{
        return state != State::NOT_ON_CRUISE;
    }
    void cruise_helper(Point destination_position, double speed);
    void cancel_if_on_cruise();
    // get the island pointer from the destination Point,
    // if no island at that destination, empty pointer is returned.
    std::shared_ptr<Island> get_first_island(Point destination);
    std::shared_ptr<Island> get_next_island();

    std::shared_ptr<Island> next_destination;
    std::shared_ptr<Island> first_island;

    std::vector<std::shared_ptr<Island>> unvisited_islands;

    constexpr static double init_fuel_capacity_c = 500.;
    constexpr static double init_max_speed_c = 15.;
    constexpr static double init_fuel_consumption_c = 2.;
    constexpr static int init_resistance_c = 0;
};

#endif
