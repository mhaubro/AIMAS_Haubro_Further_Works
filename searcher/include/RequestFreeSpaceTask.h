/*
 * RequestFreeSpaceTask.h
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_REQUESTFREESPACETASK_H_
#define SEARCHER_SRC_REQUESTFREESPACETASK_H_

#include "Task.h"
#include <utility>
#include "Location.h"

class Node;
class Agent;

class RequestFreeSpaceTask : public Task
{
public:
  int rank;
  Box * serviceBox;
  Agent * serviceAgent;
  std::list<Location> locations;//Locations to be freed

  RequestFreeSpaceTask(std::list<Location> locations, int rank, Agent * serviceAgent);
  RequestFreeSpaceTask(std::list<Location> locations, int rank, Agent * serviceAgent, Box * serviceBox);
  bool seemsCompleted(Agent * a, Node * n);
  bool isCompleted(Agent * a, Node * n);
  int h(Agent * a, Node * n);

  void popLocation();//Removes a location, if the agent is able to progress.

};

#include "Agent.h"
#include "Node.h"


#endif /* SEARCHER_SRC_REQUESTFREESPACETASK_H_ */
