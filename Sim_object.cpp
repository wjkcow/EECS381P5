#include "Sim_object.h"

#include <iostream>

using namespace std;

Sim_object::Sim_object(const string& name_): name{name_}{
    cout << "Sim_object " << name_ << " constructed" << endl;
}


Sim_object::~Sim_object(){
    cout << "Sim_object " << name << " destructed" << endl;
}
