#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <utility>
#include "Location.h"

class Command{
private:

public:
	//Constructors should only every be called from the lambda in Command.cpp
	Command(int d);
	Command(int t, int d1, int d2);
	Command();
	~Command();



	Location boxdloc();
	Location agentdloc();
	int getActionType();

	enum Dir {
		N, W, E, S, LASTD
	};

	enum Type {
		Move, Push, Pull, NoOp
	};

	//Maybe this should be refactored into pull, push and move.
	static std::vector<Command> EVERY;

	std::string toString();

private:

	static bool isOpposite(int d1, int d2);
	static int getdx(int d);
	static int getdy(int d);

	int actionType;
	int dirAgent;
	int dirBox;
};


#endif
