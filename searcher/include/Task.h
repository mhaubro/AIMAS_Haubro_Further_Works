#ifndef TASK_H
#define TASK_H

#include <utility>
#include "Location.h"
class Agent;
class Node;
class Box;

class Task
{
public:
	int rank;

	virtual bool seemsCompleted(Agent * a, Node * n);
	virtual bool isCompleted(Agent * a, Node * n);
	virtual int h(Agent * a, Node * n);
};


#include "Agent.h"
#include "Box.h"
#include "Node.h"

#endif
