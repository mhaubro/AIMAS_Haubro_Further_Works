/*
 * Plan.cpp
 *
 *  Created on: 5 May 2018
 *      Author:
 */

#include "Plan.h"

#define LOOKAHEAD 1

Plan::Plan(){
actions = std::list<Command *>();
}

std::list<Location> Plan::getLocations(){
	return locations;
}

Plan::Plan(std::list<Node *> nodes, Location startloc) {
	// TODO Auto-generated constructor stub
	actions = std::list<Command *>();
	locations = std::list<Location>();
	while (!nodes.empty()){
		actions.push_back(nodes.front()->getAction());
		nodes.pop_front();
	}

	Location newAgentloc = startloc;
	Location newTakenLoc;
	//Gets locations
	std::list<Command *>::iterator it;
	for (it = actions.begin(); it != actions.end(); ++it){
		startloc = newAgentloc;
		newAgentloc = getNewLocation(*it, newAgentloc);
		newTakenLoc = getTakenLoc(*it, startloc);
    locations.push_back(newTakenLoc);
	}

	//Maybe grab and store location
	//TODO
}

//Gets the new occupied location, based on a command.
Location Plan::getTakenLoc(Command * c, Location startloc){
	switch (c->getActionType()){
		case Command::Pull:{
			//When pulling, it is the agent that takes a new position.
			return getNewLocation(c, startloc);
		}
		case Command::Push:{
			Location boxstart = startloc + c->agentdloc();
			return boxstart + c->boxdloc();
		}
		case Command::Move:{
			return getNewLocation(c, startloc);
		}
		default:{
			return Location(0, 0);
		}
	}
}

//Gets the new location of the agent, based on a command.
Location Plan::getNewLocation(Command * c, Location newloc){
	return newloc += c->agentdloc();
}


std::string Plan::toString(){
	std::string s;
	for (Command * c : actions){
		s.append(c->toString());
	}
	s.append("\n");
	for (Location loc : locations){
		s.append(loc.toString());
	}
	s.append("\n");
	return s;
}


Plan::~Plan() {
	// TODO Auto-generated destructor stub
}

bool Plan::isEmpty(){
	return actions.size() == 0;
}

Command * Plan::getStep(){
	Command * c = actions.front();
	actions.pop_front();
	return c;
}

Location Plan::getNextLocation(){
	return locations.front();
}

void Plan::popStep(){
	locations.pop_front();
}

void Plan::popFront(){
	actions.pop_front();
}
void Plan::drain(){
	actions.clear();
}

//Checks if the plan is legal with the current lookahead.
//An undefined lookahead (Outcomment top) will lead to checking the entire plan
bool Plan::checkLegality(Node * state){
	//Not necessary at the moment


	return true;
}

bool Plan::checkLegality(Node * state, Node * tempState){
	if (state == tempState){
		return checkLegality(state);
	} else {
		return (checkLegality(state) && checkLegality(tempState));
	}
}
