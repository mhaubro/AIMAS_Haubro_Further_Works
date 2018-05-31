/*
 * getAwayFromGoalTask.cpp
 *
 *  Created on: 24 May 2018
 *      Author: martin
 */

#include "GetAwayFromGoalTask.h"

GetAwayFromGoalTask::GetAwayFromGoalTask() {
	// TODO Auto-generated constructor stub

}

GetAwayFromGoalTask::~GetAwayFromGoalTask() {
	// TODO Auto-generated destructor stub
}

bool GetAwayFromGoalTask::isCompleted(Agent * agent, Node * n)
{
	for (const auto &a : n->agents){
		if (a.getChar() == agent->getChar()){
			for(const auto &g : n->goals){
				if (a.getLocation() == g.getLocation()){
					return false;
				}
			}
		}

	}
	return true;
}


bool GetAwayFromGoalTask::seemsCompleted(Agent * a, Node * n)
{
	return isCompleted(a, n);
}


int GetAwayFromGoalTask::h(Agent * agent, Node * n)
{
	int hval = 1000.0;
	//Iterates over locations
	for (const auto &a : n->agents){
		if (a.getChar() == agent->getChar()){
			for(const auto &g : n->goals){
				if (a.getLocation() == g.getLocation()){
					hval += 5;
				}
			}
			return hval;
		}

	}
	return 10000.0;
}
