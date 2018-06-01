/*
 * Goal.h
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_GOAL_H_
#define SEARCHER_SRC_GOAL_H_

#include "Base.h"
#include "Entity.h"

class Base;

class Goal : public Entity {
public:
	Goal(Base & base, Location location);
	virtual ~Goal();
};

#endif /* SEARCHER_SRC_GOAL_H_ */
