#ifndef ISLAND_H
#define ISLAND_H

#include "Sim_object.h"
#include "Geometry.h"

#include <string>
/***** Island Class *****/
/* Islands are a kind of Sim_object; they have an amount of fuel and a an amount by which it increases
every update (default is zero). The can also provide or accept fuel, and update their amount
accordingly.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/
class Island: public Sim_object{
public:
	// initialize then output constructor message
	Island (const std::string& name_, Point position_, double fuel_ = 0., double production_rate_ = 0.);
	// output destructor message
	~Island();
		
	// Return whichever is less, the request or the amount left,
	// update the amount on hand accordingly, and output the amount supplied.
	double provide_fuel(double request);
	// Add the amount to the amount on hand, and output the total as the amount the Island now has.
	void accept_fuel(double amount);
	
	Point get_location() const override
		{return position;}

	// if production_rate > 0, compute production_rate * unit time, and add to amount, and print an update message
	void update() override;

	// output information about the current state
	void describe() const override;

	// ask model to notify views of current state
	void broadcast_current_state() override;

	// forbid  copy/move, construction/assignment
    Island(const Island&) = delete;
    Island(Island&&) = delete;
    Island& operator= (const Island&) =  delete;
    Island& operator= (Island&&)  = delete;
    
private:
	Point position;				// Location of this island
    double fuel;                // current fuel of this island
    double production_rate;     // production rate of fuel
};

#endif
