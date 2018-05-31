/*
 * HandleGoalTask.h
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_HANDLEGOALTASK_H_
#define SEARCHER_SRC_HANDLEGOALTASK_H_

#include <utility>
#include <vector>
#include "Task.h"
#include "Location.h"

class Box;
class Agent;
class Node;

class HandleGoalTask : public Task
{
public:
  std::vector<bool> solvingColors;
  std::vector<HandleGoalTask *> predecessors;//Goal that must be completed before this.

  Box * box;
  Location destination;
  int rank;
  char chr;

  double startHval;

  HandleGoalTask(Location loc, int rank, std::vector<bool> solvingColors, char chr);
  HandleGoalTask(Location loc, int rank, Box * b);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);
  bool predecessorsComplete(Agent * a, Node * n);
  bool predecessorsCompleteGoal(Goal * g, Node * n);
};

#include "Agent.h"
#include "Box.h"
#include "Node.h"


#endif /* SEARCHER_SRC_HANDLEGOALTASK_H_ */
