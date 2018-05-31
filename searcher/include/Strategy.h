#ifndef STRATEGY_H
#define STRATEGY_H
#include <list>

// predefines
class Node;
class Agent;
class Task;
class Goal;

std::list<Node *> a_star_search(Node * state, Agent* agent, Task* task);
std::list<Node *> a_star_search(Node * state, Agent* agent, Task* task, Goal * g1, Goal * g2);

#endif
