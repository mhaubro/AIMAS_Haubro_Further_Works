#include "Client.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Command.h"
#include "Initializer.h"
#include "Statics.h"

using std::stringstream;
using std::istream;
using std::vector;
using std::string;
using std::getline;
using std::pair;
using std::list;

void printStep(std::string s){
	std::stringstream ss;

	std::cout << s;
	std::cout << "\n";
	std::string response;
	std::getline(std::cin, response);

	if (response.find(std::string("false")) != std::string::npos) {
		ss << "Server responsed with "<< response <<"%s to the inapplicable action: "<< s <<"\n";
		ss.str("");
		ss << s << " was attempted in a state\n";
		throw ("Command Error, printing step\n");
	}
}

void search(){
/*
	//Yes yes it's ugly but it's needed.
	Entity::maxX = Node::maxX;
	Entity::maxY = Node::maxY;
	Location::maxX = Node::maxX;
	Location::maxY = Node::maxY;
	Node::hashGoals();

	//std::cerr << "Starts GetPlan\n";
	int agents = initialState->agents.size();

	//Get number of regions
	int regions = 0;
	for (int i = 0; i < agents; i++){
		int reg = initialState->agents[i].getRegion();
		if (reg > regions)
			regions = reg;
	}
	regions++;

	//Init centralPlanners
	std::vector<CentralPlanner> planners = std::vector<CentralPlanner>();
	//std::cerr << "Pre preanalysis\n";
	for (int i = 0; i < regions; i++){
		//Create a planner with a region
		planners.push_back(CentralPlanner(i));
		initialState->doHash();
		planners[i].preAnalyse(initialState);
		//////std::cerr << "CentralPlanner created\n";
	}
	//std::cerr << "Post preanalysis\n";

	//Give each agent a pointer to its planner
	for (int i = 0; i < agents; i++){
		Agent * agent = &initialState->agents[i];
		agent->setMyPlanner(&planners[agent->getRegion()]);
	}

	//Lazy hack to ensure that centralPlanners are completely decoupled
	std::vector<Node> states = std::vector<Node>(regions);
	std::vector<Node> tempstates = std::vector<Node>(regions);
	for (int i = 0; i < regions; i++){
		Node state = *initialState;
		state.clearOtherRegions(i);
		state.doHash();
		Node tempstate = state;

		states[i] = state;
		tempstates[i] = tempstate;

	}

	//Giving each centralplanner their node
	for (int i = 0; i < regions; i++){
		planners[i].node = &tempstates[i];
	}

	//Making a vector of agent pointers to getactions from.
	std::vector<Agent *> agentptrs = std::vector<Agent *> (agents);
	for (int j = 0; j < regions; j++){
		Node * state = &tempstates[j];
		for (int k = 0; k < state->agents.size(); k++){
			int num = state->agents[k].number;
			agentptrs[num] = &state->agents[k];
		}
	}

	while (true)
	{
		std::string s = "[";
		for (int i = 0; i < agents; i++)
		{
			int region = agentptrs[i]->getRegion();
			//s += "NoOp";
			s += (agentptrs[i]->getAction(&states[region], &tempstates[region])->toString());
			if (i == agents-1)
				s += ']';
			else
				s += ", ";
		}

		bool isGoal= false;

		for (int i = 0; i < regions; i++){
			states[i] = tempstates[i];
			states[i].doHash();
			isGoal &= states[i].isGoalState();
		}
		printStep(s);

		if (isGoal)
			return;
	}
	*/
}







int main(int argc, char * argv[]){

	Initializer::setupWorld();

	std::cout << statics->fixedState.toString();
	//Search
	search();

	return 0;
}

