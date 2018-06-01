#include "Base.h"
#include <iostream>
#include <string>

Base::Base(char chr, COLOR color, int region, int id) :
chr(chr), color(color), region(region), id(id) {

}

Base::COLOR Base::getColor() const{
	return color;
}
char Base::getChar() const{
	return chr;
}

int Base::getRegion() const{
	return region;
}

int Base::getID() const {
	return id;
}

/*double Statics::getDistance(Entity e) const{
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
*/
