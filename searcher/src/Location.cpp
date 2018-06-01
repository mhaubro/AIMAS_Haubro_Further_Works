/*
 * Location.cpp
 *
 *  Created on: 19 May 2018
 *      Author: martin
 */

#include "Location.h"
#include <cmath>
#include <sstream>

Location::Location(){
	this->x = -3;
	this->y = -3;
}

Location::Location(int x, int y){
	this->x = x;
	this->y = y;
}

Location::Location(const Location& location){
	x = location.getX();
	y = location.getY();
}

bool Location::isOutOfBounds(){
	return !(x > 0 && x < maxX && y > 0 && y < maxY);
}

int Location::maxX;
int Location::maxY;

int Location::getIndex() const{
	return x+y*maxX;
}


double Location::getDistance(Location otherLoc) const{
	return 0;
	//return CentralPlanner::allPairsShortestPaths[otherLoc.getIndex()+getIndex()*maxX*maxY];
}

int Location::getManhattan(Location otherLoc) const{
	return abs(x - otherLoc.getX()) + abs(y - otherLoc.getY());
}

std::pair<int,int> Location::getLocation(){
	return std::pair<int,int>(x,y);
}
int Location::getX() const{
	return x;
}
int Location::getY() const{
	return y;
}

void Location::setLocation(Location loc){
	this->setLocation(loc.getX(), loc.getY());
}

void Location::setLocation(int x, int y){
	this->x = x;
	this->y = y;
}
void Location::setDLocation(Location loc){
	this->x += loc.getX();
	this->y += loc.getY();
}

void Location::setDLocation(int x, int y){
	this->x += x;
	this->y += y;
}

void Location::setX(int x){
	this->x = x;
}
void Location::setY(int y){
	this->y = y;
}

Location Location::operator=(const Location& b){
	this->x = b.getX();
	this->y = b.getY();
	return *this;
}
Location Location::operator+(const Location& b) const{
	return Location(this->x + b.getX(), this->y + b.getY());
}
Location Location::operator-(const Location& b) const{
	return Location(this->x - b.getX(), this->y - b.getY());
}
Location Location::operator-(){
	return Location(this->x = -this->x, this->y - this->y);
}
Location Location::operator+=(const Location& b){
	this->setDLocation(b);
	return *this;
}
Location Location::operator-=(const Location& b){
	this->setDLocation(Location(0,0)-b);
	return *this;
}

bool Location::operator==(const Location& b) const{
	return (this->x == b.getX() && this->y == b.getY());
}

bool Location::operator!=(const Location& b) const{
	return !(this->x == b.getX() && this->y == b.getY());
}

std::string Location::toString() const{
	std::stringstream s;
	s << "(x: " << x << ", y: " << y << ")";
	return s.str();
}

// output stream function for easier printing: kind regards Mathias :)
std::ostream &operator<< (std::ostream &os, Location const& location){
  os << "[" << location.getX() << "," << location.getY() << "]";
  return os;
}
