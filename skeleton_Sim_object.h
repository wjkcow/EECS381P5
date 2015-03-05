#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H
/* This class provides the interface for all of simulation objects. It also stores the
object's name, and has pure virtual accessor functions for the object's position
and other information. */

/* *** You may not add any additional classes, structs, functions etc to this file. */
#include <string>

struct Point;

class Sim_object {
public:
    // *** define the constructor in Sim_object.cpp to output the supplied message
	Sim_object(const std::string& name_);

    // *** define the destructor in Sim_object.cpp to output the supplied message
    virtual ~Sim_object();
	
	const std::string& get_name() const
		{return name;}
    
	// ask model to notify views of current state
    virtual void broadcast_current_state() {}

	/* Interface for derived classes */
	// *** declare the following as pure virtual functions 
	Point get_location() const
	void describe() const
	void update()
	
private:
	std::string name;
};


#endif
