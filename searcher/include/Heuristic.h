
//Not in use as of right now
#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Node.h"
#include "Box.h"
#include "Agent.h"
#include <utility>
#include <string>

class Heuristic
{
public:
    ~Heuristic();
    Heuristic(Node * initialState);
  	int h(Node * n);
    int manhattan(Location loc1, Location loc2);
    virtual int f(Node *n);

	  //int compare(Node n1, Node n2);
};

class AStar : public Heuristic {
public:
    AStar(Node * initialState);
		int f(Node * n);
		std::string toString();
	};

class WeightedAStar : public Heuristic {
private:
       int W;
public:
		WeightedAStar(Node * initialState, int W);
		int f(Node *n);
		std::string toString();
	};

class Greedy : public Heuristic {
public:
  	Greedy(Node *initialState);
		int f(Node *n);
		std::string toString();

};


#endif
