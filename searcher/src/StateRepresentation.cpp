/*
 * StateRepresentation.cpp
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#include "StateRepresentation.h"
#include "Statics.h"

using std::string;

StateRepresentation::StateRepresentation() {
	// TODO Auto-generated constructor stub

}

StateRepresentation::StateRepresentation(unordered_map<Location, Box, LocationHash> boxes, unordered_map<Location, Agent, LocationHash> agents){
	this->boxes = boxes;
	this->agents = agents;
}


StateRepresentation::~StateRepresentation() {
	// TODO Auto-generated destructor stub
}

string StateRepresentation::toString(){
	string stateString;
	stateString.resize((statics->maxX+1)*statics->maxY, ' ');
	for (int y = 0; y <= statics->maxY; y++){
		for (int x = 0; x <= statics->maxX; x++){
			if (statics->walls[Location(x,y).getIndex()]){
				//Having + y for the newlines
				stateString[(statics->maxX+1) * y + x] = '+';
			}
		}
		stateString[(statics->maxX+1) * y + (statics->maxX)] = '\n';
	}

	std::cerr << stateString << "\n\n\n";

	for (auto it = agents.begin(); it != agents.end(); ++it){
		Location loc = it->first;
		stateString[(statics->maxX+1)*loc.getY()+loc.getX()] = it->second.getChar();
	}
	for (auto it = boxes.begin(); it != boxes.end(); ++it){
		Location loc = it->first;
		stateString[(statics->maxX+1)*loc.getY()+loc.getX()] = it->second.getChar();
	}
	for (auto it = statics->goals.begin(); it != statics->goals.end(); ++it){
		Location loc = it->first;
		stateString[(statics->maxX+1)*loc.getY()+loc.getX()] = it->second.getChar();
	}

	return stateString;
}
