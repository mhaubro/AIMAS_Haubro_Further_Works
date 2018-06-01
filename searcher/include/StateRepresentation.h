/*
 * StateRepresentation.h
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_STATEREPRESENTATION_H_
#define SEARCHER_SRC_STATEREPRESENTATION_H_

#include "Box.h"
#include "Agent.h"
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::unordered_map;

class StateRepresentation {
public:
	StateRepresentation();
	StateRepresentation(unordered_map<Location, Box, LocationHash> boxes, unordered_map<Location, Agent, LocationHash> agents);
	virtual ~StateRepresentation();

	unordered_map<Location, Box, LocationHash> boxes;
	unordered_map<Location, Agent, LocationHash> agents;

	string toString();


private:

};

#endif /* SEARCHER_SRC_STATEREPRESENTATION_H_ */
