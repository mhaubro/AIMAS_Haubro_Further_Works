/*
 * Plan.h
 *
 *  Created on: 5 May 2018
 *      Author:
 */

#ifndef PLAN_H_
#define PLAN_H_

#include "Node.h"
#include "Location.h"
#include <list>
#include <utility>

class Plan {
public:
	Plan(std::list<Node *>, Location startloc);
	Plan();
	~Plan();
	bool checkLegality(Node * state);

	bool checkLegality(Node * state, Node * tempState);

	bool isEmpty();

	std::string toString();

	Command * getStep();
	void popFront();
	void drain();
	void popStep();
	Location getNextLocation();
	std::list<Location> getLocations();
	std::list<Location> locations;//The locations that the plan will reach.

private:
	//std::list<Node *> nodes;
	//A pair is used here to indicate the (x,y) of a new location for each command
	std::list<Command *> actions;
	Location getTakenLoc(Command * c, Location startloc);
	Location getNewLocation(Command * c, Location newloc);
};

#endif /* SEARCHER_SRC_PLAN_H_ */
