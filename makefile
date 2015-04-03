CC = g++
LD = g++

CFLAGS = -c -pedantic-errors -std=c++11 -Wall -fno-elide-constructors
LFLAGS = -pedantic -Wall

CPPS = p4_main.cpp Sim_object.cpp Island.cpp Ship.cpp Tanker.cpp Warship.cpp Cruiser.cpp Ship_factory.cpp Controller.cpp Model.cpp View.cpp Utility.cpp Track_base.cpp Geometry.cpp Navigation.cpp Views.cpp Cruise_ship.cpp
OBJS = p4_main.o Sim_object.o Island.o Ship.o Tanker.o Warship.o Cruiser.o Ship_factory.o Controller.o Model.o View.o Utility.o Track_base.o Geometry.o Navigation.o Views.o Cruise_ship.o
PROG = p4exe

default: $(PROG)


default: $(PROG)

$(PROG): Obj
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

Obj: 
	$(LD) $(CFLAGS) $(CPPS) 

Utility.o: Utility
clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe

