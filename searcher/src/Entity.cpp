#include <Base.h>
#include <iostream>
#include <string>
#include "CentralPlanner.h"

Entity::Entity(char chr, Location location, COLOR color, int region, int id) :
chr(chr), location(location), color(color), region(region), id(id) {

}

int Entity::maxX;
int Entity::maxY;


Entity::COLOR Entity::getColor() const{
	return color;
}
char Entity::getChar() const{
	return chr;
}

int Entity::getRegion() const{
	return region;
}

int Entity::getID() const {
	return id;
}

double Entity::getDistance(Entity e) const{
	return CentralPlanner::allPairsShortestPaths[location.getIndex()+e.getLocation().getIndex()*maxX*maxY];
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
