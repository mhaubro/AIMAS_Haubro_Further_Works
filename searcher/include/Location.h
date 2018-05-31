/*
 * Location.h
 *
 *  Created on: 19 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_LOCATION_H_
#define SEARCHER_SRC_LOCATION_H_

#include <utility>
#include <string>
#include <iostream>


class Location {
public:

	static int maxX;
	static int maxY;

	bool isOutOfBounds();

	Location();
	Location(int x, int y);
	Location(const Location& location);

	int getIndex() const;

	std::pair<int,int> getLocation();
	int getX() const;
	int getY() const;

	void setLocation(Location loc);
	void setLocation(int x, int y);
	void setDLocation(Location loc);
	void setDLocation(int x, int y);

	double getDistance(Location otherLoc) const;
	int getManhattan(Location otherLoc) const;

	void setX(int x);
	void setY(int y);

	Location operator=(const Location& b);
	Location operator+(const Location& b) const;
	Location operator-(const Location& b) const;
	Location operator+=(const Location& b);
	Location operator-=(const Location& b);
	bool operator==(const Location& b) const;
  bool operator!=(const Location& b) const;

	Location operator-();//Unary

	std::string toString() const;

private:
	int x;
	int y;
};

struct LocationHash {
public:
	size_t operator()(const Location &b) const {
		return b.getX()*Location::maxY + b.getX();
	}
};


/*
 * Necessary for comparing using the std::unordered set
 */
struct LocationEqual {
public:
	bool operator()(const Location & b1, const Location & b2) const {
		return b1 == b2;
	}
};

// output stream function for easier printing: kind regards Mathias :)
std::ostream &operator<< (std::ostream &os, Location const& location);

#endif /* SEARCHER_SRC_LOCATION_H_ */
