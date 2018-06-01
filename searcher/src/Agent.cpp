/*
 * Agent.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: martin
 */

#include "Agent.h"

Agent::Agent(Base & base, Location loc) : Entity(base, loc) {
	number = base.getChar() - '0';
}

Agent::~Agent() {
	// TODO Auto-generated destructor stub
}

