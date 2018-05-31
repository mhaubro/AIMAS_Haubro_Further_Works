/*
 * HandleGoalTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "HandleGoalTask.h"
#include "Location.h"
#include <cmath>

HandleGoalTask::HandleGoalTask(Location loc, int rank, std::vector<bool> solvingColors, char chr)
{
	this->solvingColors = solvingColors;
	this->predecessors = std::vector<HandleGoalTask *>();
	this->chr = chr;
	//  this->type = Task::Type::MoveBoxTask;
	this->box = NULL;
	this->destination = loc;
	this->rank = rank;
	this->startHval = 1000;
}

HandleGoalTask::HandleGoalTask(Location loc, int rank, Box * box)
{
	this->solvingColors = std::vector<bool>();
	this->predecessors = std::vector<HandleGoalTask *>();
	//  this->type = Task::Type::MoveBoxTask;
	this->box = box;
	this->chr = box->getChar();
	this->destination = loc;
	this->rank = rank;
	this->startHval = 1000;
}

bool HandleGoalTask::isCompleted(Agent * a, Node * n)
{
	for(auto & b : n->boxes)
	{
		if(b.getChar() != this->box->getChar())
			continue;
		if(b.getLocation() == this->destination)
		{
			return true;
		}
	}
	return false;
}

bool HandleGoalTask::seemsCompleted(Agent * a, Node * n)
{
	for(auto & b : n->boxes)
	{
		if(this->box && b.getChar() != this->box->getChar())
			continue;
		if(b.getLocation() == this->destination)
			return true;
	}
	return false;
}

int HandleGoalTask::h(Agent * a, Node * n)
{
	//Finding right agent:
	Agent * activeAgent;
	for (int i = 0; i < n->agents.size(); i++){
		if (n->agents[i].number == a->number){
			activeAgent = &n->agents[i];
			break;
		}
	}


  double hval = this->startHval;

	for (Box b : n->boxes)
  {
		if (b.getColor() != a->getColor())
			continue;

		if (Goal * g = n->getGoal(b.getLocation()))
    {
			if (g->getChar() == tolower(b.getChar()) && predecessorsCompleteGoal(g, n))
      {
          if(a->getPlanner()->isFree(n, b.getLocation())){
            // If it is a good position i.e. can not block reward it
            hval -= 20;
          } else{
            // If not good reward it less
            hval -= 10;
          }
			} 
		}

    // The box in this task
		if (b.getID() == box->getID())
    {
			hval += destination.getDistance(b.getLocation());
			hval += b.getDistance(*activeAgent);
			if (b.getDistance(*activeAgent) < 1.3){
				hval -= 5.0;
      }
      
		}
	}
	return n->g() * 0.5f + hval;
}

bool HandleGoalTask::predecessorsComplete(Agent * a, Node * n)
{
  if(this->predecessors.size() != 0)
  {
    for(int j = 0; j < this->predecessors.size(); j++)
    {
      if(!this->predecessors[j]->seemsCompleted(a, n))
        return false;
    }
  }
  return true;
}

// This is used for goal 
bool HandleGoalTask::predecessorsCompleteGoal(Goal * g, Node * n)
{
	for (int i = 0; i < g->predecessors.size(); i++){
  	if(!n->isGoalState(g->predecessors[i])){
			return false;
		}
	}
	return true;
}
