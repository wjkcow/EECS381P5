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
    // move like a ship or cruise
    void update() override;
    void describe() const override;
private:
    // cruise is always on this speed
    double cruise_speed;
    // next island we will visit
    std::shared_ptr<Island> next_destination;
    
    // first island to cruise to
    std::shared_ptr<Island> first_island;
    
    // all the island we have no visited
    std::vector<std::shared_ptr<Island>> unvisited_islands;
    enum class State;
    State cruise_ship_state;

    void cancel_if_on_cruise();
    // get the island pointer from the destination Point,
    // if no island at that destination, empty pointer is returned.
    std::shared_ptr<Island> get_island_at_location(Point destination);
    // get the next island to cruise to depend on next_island and remove the island from unvisited islands
    std::shared_ptr<Island> get_next_island_and_remove();

};

#endif
