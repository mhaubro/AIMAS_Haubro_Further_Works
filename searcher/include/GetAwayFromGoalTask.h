/*
 * getAwayFromGoalTask.h
 *
 *  Created on: 24 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_GETAWAYFROMGOALTASK_H_
#define SEARCHER_SRC_GETAWAYFROMGOALTASK_H_

#include <utility>
#include <vector>
#include "Task.h"
#include "Location.h"

class Box;
class Agent;
class Node;

class GetAwayFromGoalTask : public Task{
public:
	GetAwayFromGoalTask();
	virtual ~GetAwayFromGoalTask();

	bool isCompleted(Agent * a, Node * n);
	bool seemsCompleted(Agent * a, Node * n);
	int h(Agent * a, Node * n);
};

#endif /* SEARCHER_SRC_GETAWAYFROMGOALTASK_H_ */
