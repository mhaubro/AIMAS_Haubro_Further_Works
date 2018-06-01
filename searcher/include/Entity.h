/*
 * Entity.h
 *
 *  Created on: 1 Jun 2018
 *      Author: martin
 */

#ifndef SEARCHER_INCLUDE_ENTITY_H_
#define SEARCHER_INCLUDE_ENTITY_H_

#include "Base.h"
#include "Location.h"

class Base;

class Entity {
public:

	Entity(Base & base, Location location);
	virtual ~Entity();

	double getDistance(Entity e) const;
	int getManhattan(Entity e) const;
	void setLocation(Location loc);
	void setDLocation(Location loc);
	Location getLocation() const;

	char getChar();

	Base & getBase();

private:
	Location location;
	Base & base;
};

#endif /* SEARCHER_INCLUDE_ENTITY_H_ */
