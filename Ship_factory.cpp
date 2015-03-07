#include "Ship_factory.h"

#include "Tanker.h"
#include "Warship.h"
#include "Cruiser.h"

Ship* create_ship(const std::string& name, const std::string& type, Point initial_position){
    if (type == "Tanker") {
        return new Tanker(name, initial_position);
    } else if (type == "Warship") {
        return new Cruiser(name, initial_position);
    } else {
        throw Error("Trying to create ship of unknown type!");
    }
}

