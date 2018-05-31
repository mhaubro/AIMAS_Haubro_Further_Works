/*
 * Statics.h
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_STATICS_H_
#define SEARCHER_SRC_STATICS_H_

#include "StateRepresentation.h";
#include "Base.h"
#include "Goal.h"
#include "Actor.h"
#include <vector>

class Statics {
public:
	Statics();
	virtual ~Statics();

	static const int maxX;
	static const int maxY;

	static StateRepresentation runningState;
	static StateRepresentation fixedState;

	static const std::vector<Actor> actors;
	static const std::vector<Goal> goals;

	static const std::vector<Base> bases;

	static const std::vector<int> allPairsShortestPath;



};

#endif /* SEARCHER_SRC_STATICS_H_ */
