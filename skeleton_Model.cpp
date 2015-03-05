
/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */

	new Island("Exxon", Point(10, 10), 1000, 200)
	new Island("Shell", Point(0, 30), 1000, 200)
	new Island("Bermuda", Point(20, 20))
	
	create_ship("Ajax", "Cruiser", Point (15, 15))
	create_ship("Xerxes", "Cruiser", Point (25, 25))
	create_ship("Valdez", "Tanker", Point (30, 30))

	cout << "Model constructed" << endl;

