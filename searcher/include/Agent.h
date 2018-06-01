/*
 * Agent.h
 *
 *  Created on: 1 Jun 2018
 *      Author: martin
 */

#ifndef SEARCHER_INCLUDE_AGENT_H_
#define SEARCHER_INCLUDE_AGENT_H_

#include "Base.h"
#include "Entity.h"

class Base;

class Agent : public Entity{
public:
	int number;

	Agent(Base & base, Location loc);
	virtual ~Agent();
};

#endif /* SEARCHER_INCLUDE_AGENT_H_ */
