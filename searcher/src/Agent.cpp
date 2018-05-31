#include "Agent.h"
#include "Task.h"
#include "HandleGoalTask.h"
#include <functional>
#include <utility>
#include "Entity.h"
#include "Strategy.h"
#include "CentralPlanner.h"
#include "RequestFreeSpaceTask.h"
#include "GetAwayFromGoalTask.h"
#include <iostream>

std::list<Node *> Agent::search(Node * state){
	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		//There's an agent on our destination
		if (Agent * a = state->getAgent(tmp->destination)){
			if (a->getChar() != chr){
				Node stateWithoutAgent = *state;
				stateWithoutAgent.removeAgent(tmp->destination);

				//This should be a function
				std::list<Location> loc = std::list<Location>();
				loc.push_back(tmp->destination);
				myPlanner->removeRequestTask(freeSpaceTask);
				RequestFreeSpaceTask * req = new RequestFreeSpaceTask(loc, 0, this, tmp->box);
				if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
					delete req;
				} else {
					delete freeSpaceTask;
					myPlanner->removeRequestTask(freeSpaceTask);
					freeSpaceTask = req;
					myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
				}
				return a_star_search(&stateWithoutAgent, this, this->task);
			}
		}
		if (Box * b = state->getBox(tmp->destination)){
			if (b->getColor() != color){
				Node stateWithoutBox = *state;
				stateWithoutBox.removeBox(tmp->destination);

				//This should be a function
				std::list<Location> loc = std::list<Location>();
				loc.push_back(tmp->destination);
				myPlanner->removeRequestTask(freeSpaceTask);
				RequestFreeSpaceTask * req = new RequestFreeSpaceTask(loc, 0, this, tmp->box);
				if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
					delete req;
				} else {
					delete freeSpaceTask;
					myPlanner->removeRequestTask(freeSpaceTask);
					freeSpaceTask = req;
					myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
				}


				return a_star_search(&stateWithoutBox, this, this->task);
				//There's a box we can't move at destination
			}
		}
	}
	return a_star_search(state, this, this->task);
}

//Commits a search where all agents are gone, and asks for the locations.
std::list<Node *> Agent::Nakedsearch(Node * state){
	Node nakedstate = *state;
	nakedstate.clearOtherAgents(chr);

	return search(&nakedstate);
}

//Commits a search where all things are gone, and asks for the locations.
std::list<Node *> Agent::noBoxesOrAgents(Node * state, Box * box){
	Node nakedstate = *state;
	nakedstate.clearOtherAgentsAndBoxes(chr, box);
	return search(&nakedstate);
}

void Agent::cleanTasks(){
	if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		myPlanner->returnGoalTask(tmp);
		tmp->box->workInProgress = false;
	} else 	if (RequestFreeSpaceTask* tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
		if (!myPlanner->stillActiveRequest(tmp)){
			task = NULL;
		}
	} else 	if (GetAwayFromGoalTask* tmp = dynamic_cast<GetAwayFromGoalTask*>(this->task)){
		delete task;
		task = NULL;
	}
	removeFreeSpaceTask();
	//t should be deleted, but that would break everything right now
	//TODO
}

void Agent::removeFreeSpaceTaskMessage(RequestFreeSpaceTask * t){
	if (task == t){
		task = NULL;
		plan->drain();
	}
}

void Agent::removeFreeSpaceTask(){
	myPlanner->removeRequestTask(freeSpaceTask);
	delete freeSpaceTask;
	freeSpaceTask = NULL;
}

void Agent::replanTask(Node * state){
	std::list<Node *> searchResult;

	if (RequestFreeSpaceTask * tmp = dynamic_cast<RequestFreeSpaceTask*>(this->task)){
		//We won't be bothered
		this->task = NULL;
		return;
	} else if (HandleGoalTask* tmp = dynamic_cast<HandleGoalTask*>(this->task)){
		//Takes out all agents
		searchResult = Nakedsearch(state);

		//No boxes or agents, try path
		if (searchResult.empty()){
			searchResult = noBoxesOrAgents(state, tmp->box);
			if (searchResult.empty()){
				myPlanner->returnGoalTask((HandleGoalTask *)task);
				this->task = NULL;
				return;
			}
			//We have a path without agents and boxes, we proceed
		}
		//We have a path without agents, we proceed
		plan = new Plan(searchResult, this->getLocation());

		RequestFreeSpaceTask * req = new RequestFreeSpaceTask(plan->getLocations(), rank, this, tmp->box);
		if (freeSpaceTask && (req->locations == freeSpaceTask->locations)){
			delete req;
		} else {
			delete freeSpaceTask;
			myPlanner->removeRequestTask(freeSpaceTask);
			freeSpaceTask = req;
			myPlanner->addRequestFreeSpaceTask(freeSpaceTask);
		}

	}
	Node::resetPool();
}

void Agent::noTask(Node * startstate){
	task = new GetAwayFromGoalTask();
	plan = new Plan(search(startstate), this->getLocation());
}

void Agent::gettingJob(Node * startstate)
{
	if (myPlanner->hasJob(this, startstate))
  {
		task = myPlanner->getJob(this, startstate);
		std::list<Node *> searchResult = search(startstate);
		if (searchResult.empty())
    {
			return;
			//Do something
		}
		plan = new Plan(searchResult, this->getLocation());
	}
}

void Agent::noPlan(Node * startstate){

	if (task == NULL){
		task = myPlanner->getJob(this, startstate);
	}

	//If we still don't have a task
	if (task == NULL){
		task = new GetAwayFromGoalTask();
		if (task->isCompleted(this, startstate)){
			delete task;
			task = NULL;
		}
	}

	std::list<Node *> searchResult = search(startstate);
	if (searchResult.empty()){
		//We check if we can help. If not, we ask for help ourselfes.
		if (checkForHelp(startstate)){
			//We can help. Let's help.
			searchResult = search(startstate);
			plan = new Plan(searchResult, location);
		} else {
			//Let's see if we can solve our current task.
			replanTask(startstate);
		}


	} else {
		//We found a way to handle our current task.
		delete plan;
		removeFreeSpaceTask();
		plan = new Plan(searchResult, location);
	}
}

void Agent::maybeSleep(int sleep){
		double prob = 0.3;
		if (((double)rand())/RAND_MAX + prob > 1){
			skipNextIte = sleep;
		}
}

bool Agent::checkForHelp(Node * state){
	if (myPlanner->hasHelpJob(this, state) && !myPlanner->stillActiveRequest((RequestFreeSpaceTask *) task)){
		cleanTasks();
		task = myPlanner->getHelpJob(this, state);
		plan->drain();
		return true;
	}
	return false;
}

Command * Agent::getAction(Node * startstate, Node * tempstate){

	if (skipNextIte){
		skipNextIte--;
		return &Command::EVERY[0];
	}
	//Checks if we have a plan, and the task is still valid
	if (plan != NULL && !plan->isEmpty() && task != NULL){
		//Plan seems to be legal
		Command * c = plan->getStep();
		if (c == NULL){
			//This should be a sanity check
			plan->drain();
			return &Command::EVERY[0];
		}

		if (!startstate->checkState(number, c)){
			noPlan(tempstate);
			maybeSleep(2);
			return &Command::EVERY[0];
		}
		if (!tempstate->checkAndChangeState(number, c)){
			noPlan(tempstate);
			maybeSleep(2);
			return &Command::EVERY[0];
		}

		plan->popStep();
		return c;

	} else {
		//We did not have a legal plan
		noPlan(tempstate);
	}

	return &Command::EVERY[0];
}

Agent::Agent(char chr, int rank, Location location, COLOR color, int region, int id):
		Entity(chr, location, color, region, id)
{
	this->task = nullptr;
	this->rank = rank;
	this->number = (int)(chr - '0');
	this->myPlanner = nullptr;
	plan = NULL;
	freeSpaceTask = NULL;

}

Agent::Agent(char chr, Location location, COLOR color, int region, int id):
		Entity(chr, location, color, region, id)
{
	this->task = nullptr;
	this->rank = 0;
	this->number = chr - '0';
	this->myPlanner = nullptr;
	plan = NULL;
	freeSpaceTask = NULL;
}

//No color, for single agent levels
Agent::Agent(char chr, Location location, int region, int id):
		Entity(chr, location, Entity::BLUE, region, id)
{
	this->task = nullptr;
	this->rank = 0;
	this->number = chr - '0';
	this->myPlanner = nullptr;
	plan = NULL;
	freeSpaceTask = NULL;
}

Agent::Agent(const Agent * agt):
		Entity(agt->getChar(), agt->getLocation(), agt->getColor(), agt->getRegion(), agt->getID())
{
	this->task = agt->task;
	this->rank = agt->rank;
	this->plan = agt->plan;
	this->number = chr - '0';
	this->myPlanner = agt->getPlanner();
	freeSpaceTask = NULL;
}

int Agent::hashCode()
{
	int result = 17;
	result = 31 * result + (int) chr;
	result = 31 * result + rank;
	result = 31 * result + location.getX();
	result = 31 * result + location.getY();
	result = 31 * result + color;
	return result;
}

void Agent::setMyPlanner(CentralPlanner * planner){
	myPlanner = planner;
}

CentralPlanner * Agent::getPlanner() const{
	return myPlanner;
}

bool Agent::equals(const Agent * agent) const
{
	if (agent == this) return true;
	return agent->getChar() == chr && agent->rank == rank && agent->getColor() == color && agent->getLocation() == location;
}
