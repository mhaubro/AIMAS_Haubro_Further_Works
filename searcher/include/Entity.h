
#ifndef ENTITY_H
#define ENTITY_H
#include <utility>
#include "Location.h"
/*
Base class for goals, agents and boxes
 */
class Entity {

public:
	enum COLOR {BLUE, RED, GREEN, CYAN, MAGENTA, ORANGE, PINK, YELLOW, NUMCOLS};

	static int maxX;
	static int maxY;


	Entity(char chr, Location location, COLOR color, int region, int id);
	//The chr is

	Entity::COLOR getColor() const;
	char getChar() const;
	int getRegion() const;

	int getID() const;

	double getDistance(Entity e) const;
	int getManhattan(Entity e) const;

	void setLocation(Location loc);
	void setDLocation(Location loc);
	Location getLocation() const;

protected:
	char chr;
	Location location;
	COLOR color;

	int region;
	int id;

};




#endif
