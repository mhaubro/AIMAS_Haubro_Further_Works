/*
 * Actor.h
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_ACTOR_H_
#define SEARCHER_SRC_ACTOR_H_

#include "Agent.h"

class Actor : public Agent {
public:
	Actor(Agent & agent);
	virtual ~Actor();
};

#endif /* SEARCHER_SRC_ACTOR_H_ */
