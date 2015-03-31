#include "Ship_factory.h"

#include "Tanker.h"
#include "Cruiser.h"
#include "Cruise_ship.h"
#include "Utility.h"

using namespace std;

shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, Point initial_position){
    if (type == "Tanker") {
        return make_shared<Tanker>(name, initial_position);
    } else if (type == "Cruiser") {
        return make_shared<Cruiser>(name, initial_position);
    } else if (type == "Cruise_ship"){
        return make_shared<Cruise_ship>(name, initial_position);
    } else {
        throw Error("Trying to create ship of unknown type!");
    }
}
