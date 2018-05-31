#ifndef Client_H
#define Client_H
#include "Node.h"
//#include "Strategy.h"

#include <iostream>
#include <list>

class Client
{
public:
	Client();
	~Client();

	//std::list<Node *> search(Strategy * strategy, int printfrequency);

	Node * initialState;

};

#endif
