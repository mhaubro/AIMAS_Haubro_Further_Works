
#include "Command.h"
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <iostream>
#include <sstream>
/*
	Initializing EVERY through a lambda expression. Requires C++-11
	This works, gives every single possible command except NoOp.
	TODO: Add NOOP. Maybe add other commands as well.
 */
std::vector<Command> Command::EVERY = []() -> std::vector<Command > {
	std::list<Command *> cmds = std::list<Command *>();
	for (int d1 = 0; d1 < Command::LASTD; d1++) {
		for (int d2 = 0; d2 < Command::LASTD; d2++) {
			if (!Command::isOpposite(d1, d2)) {
				cmds.push_back(new Command(1, d1, d2));
			}
		}
	}
	for (int d1 = 0; d1 < Command::LASTD; d1++) {
		for (int d2 = 0; d2 < Command::LASTD; d2++) {
			if (d1 != d2) {
				cmds.push_back(new Command(2, d1, d2));
			}
		}
	}
	for (int d = 0; d < Command::LASTD; d++) {
		cmds.push_back(new Command(d));
	}
	cmds.push_back(new Command(Command::LASTD));
	int elements = cmds.size();
	std::vector<Command> com(elements);
	for (int i = 0; i < elements; i++){
		com[i] = *(cmds.back());
		cmds.pop_back();
	}

	return com;
}();//Lambda ends here

bool Command::isOpposite(int d1, int d2) {
	return d1 + d2 == 3;
}

int Command::getActionType(){
	return actionType;
}


//Gives the change in a boxes location
Location Command::boxdloc(){
	int dy = getdy(this->dirBox);
	int dx = getdx(this->dirBox);
	if (this->actionType == Command::Pull){
		return Location(-dx, -dy);
	} else if (this->actionType == Command::Push) {
		return Location(dx, dy);
	} else {
		return Location(0,0);
	}
}

Location Command::agentdloc(){
	int dy = getdy(this->dirAgent);
	int dx = getdx(this->dirAgent);
	return Location(dx, dy);
}


int Command::getdy(int d) {
	// South is down one row (1), north is up one row (-1).
	switch (d) {
	case S:
		return 1;
	case N:
		return -1;
	default:
		return 0;
	}
}

Command::Command(){
	actionType = -1;
	dirAgent = -1;
	dirBox = -1;
}

int Command::getdx(int d) {
	// East is right one column (1), west is left one column (-1).
	switch (d) {
	case E:
		return 1;
	case W:
		return -1;
	default:
		return 0;
	}
}

Command::Command(int d) {
	if (d == Command::LASTD){
		this->actionType = Command::NoOp;
		this->dirAgent = -1;
	} else {
		this->actionType = Command::Move;
		this->dirAgent = d;
	}
	this->dirBox = -1;//Indicates illegal direction
}

Command::Command(int t, int d1, int d2) {
	this->actionType = t;
	this->dirAgent = d1;
	this->dirBox = d2;
}

//Destructor
Command::~Command(){

}

std::string Command::toString() {
	static const char DirNames[4][1+1] = {"N", "W", "E", "S"};
	static const char TypeNames[4][4+1] = {"Move", "Push", "Pull", "NoOp"};
	std::stringstream ss;
	char newC[12];
	//A switch would be more beautiful?
	if (this->actionType == Command::NoOp){
		ss << TypeNames[this->actionType];
		return ss.str();
	}
	else if (this->actionType == Command::Move){
		//Command will be 5 chars long

		ss << TypeNames[this->actionType] << "(" << DirNames[this->dirAgent] << ")";

		return ss.str();
	} else {

		ss << TypeNames[this->actionType] << "(" << DirNames[this->dirAgent] << "," << DirNames[this->dirBox] << ")";
		return ss.str();
	}
}
