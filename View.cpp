#include "View.h"

#include "Geometry.h"
#include <iostream>

using namespace std;
// update functions
// If the name is already present,the new info replaces the previous one.
// update the object's location
void View::update_location(const std::string& name, Point location){}

View::~View(){}

View::Cout_saver::Cout_saver(){
    old_settings = cout.flags();
    old_precision = static_cast<int>(cout.precision());
}

View::Cout_saver::~Cout_saver(){
    cout.flags(old_settings);
    cout.precision(old_precision);
}
