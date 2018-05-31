/*
 * RequestFreeSpaceTask.cpp
 *
 *  Created on: 17 May 2018
 *      Author: martin
 */

#include "RequestFreeSpaceTask.h"
#include <vector>

RequestFreeSpaceTask::RequestFreeSpaceTask(std::list<Location> locations, int rank, Agent * serviceAgent, Box * serviceBox)
{
	//this->type = Task::Type::MoveAgentTask;
	this->locations = locations;
	this->rank = rank;
	this->serviceBox = serviceBox;
	std::list<Location>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
	}
	this->serviceAgent = serviceAgent;
}

RequestFreeSpaceTask::RequestFreeSpaceTask(std::list<Location> locations, int rank, Agent * serviceAgent)
{
	std::list<Location>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
	}
	//this->type = Task::Type::MoveAgentTask;
	this->locations = locations;
	this->rank = rank;
	this->serviceBox = NULL;
	this->serviceAgent = serviceAgent;
}

bool RequestFreeSpaceTask::isCompleted(Agent * a, Node * n)
{
	std::list<Location>::iterator it;
	for(it = locations.begin(); it != locations.end(); ++it){
		if (Agent * agent = n->getAgent(*it)){
			if (a->getChar() == agent->getChar() && agent->getChar() != serviceAgent->getChar()){
				return false;
			}
		} else if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor()){
				if (serviceBox){
					if (b->getChar() != serviceBox->getChar())
						return false;
				} else {
					return false;
				}
			}
		}
	}

	return true;//
}

void RequestFreeSpaceTask::popLocation(){
	locations.pop_front();
}

bool RequestFreeSpaceTask::seemsCompleted(Agent * a, Node * n)
{
	return isCompleted(a, n);
}


int RequestFreeSpaceTask::h(Agent * a, Node * n)
{
	//std::cerr << "Trying to do a requestfreespacetask\n";
	int hval = 1000.0;
	n->doHash();
	//Iterates over locations
	std::list<Location>::iterator it;
	bool isBoxOnPath = false;
	Agent * agent;
	for (int i = 0; i < n->agents.size(); i++){
		if (a->getID() == n->agents[i].getID()){
			agent = &n->agents[i];
			break;
		}
	}

	for(it = locations.begin(); it != locations.end(); ++it){
		if (Box * b = n->getBox(*it)){
			if (b->getColor() == a->getColor()){
				if (!(serviceBox && (b->getChar() == serviceBox->getChar())))
					hval += 1 + b->getLocation().getDistance(agent->getLocation());
			}
		}
	}

	if (!isBoxOnPath){
		hval -= 20.0;//This is a really good thing
		for(it = locations.begin(); it != locations.end(); ++it){
			if (*it == agent->getLocation()){
				hval = hval + .9;
			}
		}
	}



	return hval;
}
