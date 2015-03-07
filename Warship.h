/* Warship class
A Warship is a ship with firepower and range member variables, and some services for
protected classes to manage many of the details of warship behavior. This is an
abstract base class, so concrete classes derived from Warship must be declared.
*/

/* 
This skeleton file shows the required public and protected interface for the class, 
which you may not modify.
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class 
must also have them and use them as intended.
You may define simple reader functions in the class declaration if you wish 
by changing the prototype to the definition. Your .h file for a component will always
be kept together with your .cpp file for the component.
You should delete this comment.
*/


public:
	// initialize, then output constructor message
	Warship(const std::string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_,
		int firepower_, double maximum_range_);

	// a pure virtual function to mark this as an abstract class, 
	// but defined anyway to output destructor message
	virtual ~Warship() = 0;
	
	// perform warship-specific behavior
	void update() override;

	// Warships will act on an attack and stop_attack command

	// will	throw Error("Cannot attack!") if not Afloat
	// will throw Error("Warship may not attack itself!")
    // if supplied target is the same as this Warship
	void attack(Ship* target_ptr_) override;

	// will throw Error("Was not attacking!") if not Attacking
	void stop_attack() override;
	
	void describe() const override;

protected:
	// future projects may need additional protected members

	// return true if this Warship is in the attacking state
	bool is_attacking() const;
	
	// fire at the current target
	void fire_at_target();
		
	// is the current target in range?
	bool target_in_range() const;

	// get the target
	Ship* get_target() const;

