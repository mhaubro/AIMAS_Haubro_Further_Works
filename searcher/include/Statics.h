/*
 * Statics.h
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_STATICS_H_
#define SEARCHER_SRC_STATICS_H_

#include "StateRepresentation.h"
#include "Base.h"
#include "Goal.h"
#include "Actor.h"
#include <vector>

using std::vector;

class Statics {
public:

	Statics();
	Statics(unordered_map<Location, Goal, LocationHash> goals, vector<Base> bases, vector<bool> walls, int maxX, int maxY, StateRepresentation startstate);
	virtual ~Statics();

	StateRepresentation runningState;
	StateRepresentation fixedState;
	const int maxX;
	const int maxY;

	vector<Actor> actors;
	unordered_map<Location, Goal, LocationHash> goals;

	const vector<Base> bases;
	const vector<bool> walls;
	const vector<int> allPairsShortestPath;

private:

	vector<Actor> computeActors();
	vector<int> computeAllPairsShortestPath();

};



extern Statics * statics;



#endif /* SEARCHER_SRC_STATICS_H_ */
