#ifndef ENTITY_H
#define ENTITY_H
#include <utility>
#include "Location.h"

/*
Base class for goals, agents and boxes. All agents/boxes/goals have one base.
Can not be edited.
 */

class Base {

public:
	enum COLOR : short {BLUE, RED, GREEN, CYAN, MAGENTA, ORANGE, PINK, YELLOW, NUMCOLS};

	Base(char chr, Location location, COLOR color, int region, int id);
	Base::COLOR getColor() const;
	char getChar() const;
	int getRegion() const;
	int getID() const;

protected:
	char chr;
	COLOR color;
	int region;
	int id;

};




#endif
