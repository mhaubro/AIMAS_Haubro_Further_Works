/*
 * Entity.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: martin
 */

#include "Entity.h"

Entity::Entity(Base & base, Location location) :
base(base), location(location)
{

}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

Base & Entity::getBase(){
	return base;
}

double Entity::getDistance(Entity e) const{
	return 0;
	//return CentralPlanner::allPairsShortestPaths[location.getIndex()+e.getLocation().getIndex()*maxX*maxY];
}

int Entity::getManhattan(Entity e) const{
	return location.getManhattan(e.getLocation());
}

void Entity::setLocation(Location loc){
	location = loc;
}
void Entity::setDLocation(Location loc){
	location += loc;
}

Location Entity::getLocation() const
{
	return location;
}

char Entity::getChar(){
	return getBase().getChar();
}

