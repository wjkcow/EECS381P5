//
///* create some islands and ships using the following code in the Model constructor.
//Do not change the execution order of these code fragments. You should delete this comment. */
//
//	new Island("Exxon", Point(10, 10), 1000, 200)
//	new Island("Shell", Point(0, 30), 1000, 200)
//	new Island("Bermuda", Point(20, 20))
//	
//	create_ship("Ajax", "Cruiser", Point (15, 15))
//	create_ship("Xerxes", "Cruiser", Point (25, 25))
//	create_ship("Valdez", "Tanker", Point (30, 30))
//
//	cout << "Model constructed" << endl;
//

#include "Model.h"

#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Ship_factory.h"

using namespace std;
Model::Model(){

}

Model::~Model(){

}

bool Model::is_name_in_use(const string& name) const{

}

bool Model::is_island_present(const string& name) const{

}

Island* Model::get_island_ptr(const string& name) const{

}

bool Model::is_ship_present(const string& name) const{

}

void Model::add_ship(Ship* ship_ptr){

}

Ship* Model::get_ship_ptr(const string& name) const{

}

void Model::describe() const{
}

void Model::update(){

}

void Model::attach(View* v){

}

void Model::detach(View* v){
}

void Model::notify_location(const string& name, Point location){

}

void Model::notify_gone(const string& name){

}



