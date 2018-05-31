#include "Node.h"
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
#include <algorithm>
#include <random>
#include <functional>
#include <iostream>
#include <cctype>
#include <iomanip>
#include <utility>
#include "Goal.h"
#include "Agent.h"
#include "ManualNodePool.h"
//#include <boost/pool/object_pool.hpp>
//Very possibly we should do our own, considering we don't have to "destruct" node *.
//boost::object_pool<Node> Node::pool;

/*
 * Removes boxes & agents for all other regions, so the node only contains that regions stuff
 * Maybe goals should be removed here as well, but currently it only matters for preanalysis,
 * where this is being handled by not making HandleGoalTasks when the goal is of a different
 * region.
 */
void Node::clearOtherRegions(int region){
	std::vector<Agent> newA = std::vector<Agent>();
	std::vector<Box> newB = std::vector<Box>();
	for (const auto & a : agents){
		if (a.getRegion() == region){
			newA.emplace_back(&a);
		}
	}
	for (const auto & b : boxes){
		if (b.getRegion() == region){
			newB.push_back(b);
		}
	}
	agents = newA;
	boxes = newB;
}

void Node::clearOtherAgentsAndBoxes(char agent, Box * box){
	std::vector<Agent> newA = std::vector<Agent>();
	std::vector<Box> newB = std::vector<Box>();
	for (const auto & a : agents){
		if (a.getChar() == agent){
			newA.emplace_back(&a);
		}
	}
	newB.push_back(getBox(box->getLocation()));
	agents = newA;
	boxes = newB;
	doHash();
}

void Node::clearOtherAgentsKeepBoxes(char agent, Goal * g1, Goal * g2)
{
	std::vector<Agent> newA = std::vector<Agent>();
	std::vector<Box> newB = std::vector<Box>();

	for (const auto & a : agents)
		if (a.getChar() == agent)
      newA.emplace_back(&a);

  for (const auto & b : boxes)
    if(g1->getChar() == std::tolower(b.getChar()) || g2->getChar() == std::tolower(b.getChar()))
      newB.push_back(b);

	agents = newA;
	boxes = newB;
	doHash();
}

void Node::removeBox(Location loc){
	std::vector<Box>::iterator it = boxes.begin();
	for (int i = 0; i < boxes.size(); i++){
		if (boxes[i].getLocation() == loc){
			boxes.erase(it+i);
			return;
		}
	}
	doHash();
}

void Node::removeAgent(Location loc){
	std::vector<Agent>::iterator it = agents.begin();
	for (int i = 0; i < agents.size(); i++){
		if (agents[i].getLocation() == loc){
			agents.erase(it+i);
			return;
		}
	}
}

void Node::clearOtherAgents(char agent){
	std::vector<Agent> newA = std::vector<Agent>();
	for (const  auto & a : agents){
		if (a.getChar() == agent){
			newA.emplace_back(&a);
		}
	}
	agents = newA;
	doHash();
}

std::vector<Location> Node::recordAgentLocations()
{
  std::vector<Location> locations = std::vector<Location>();
  for (const auto & a : this->agents)
    locations.push_back(a.getLocation());
  return locations;
}

void Node::resetAgent(std::vector<Location> locations)
{
  if(this->agents.size() == locations.size())
  {
    for(int i = 0; i < this->agents.size(); i++)
      this->agents[i].setLocation(locations[i]);
  }
}

void Node::resetPool(){
	pool.clearNodes();
}

Node * Node::getopCopy(Node * n){
	return pool.createNodeCopy(n, n->agents, n->boxes);
}

Node::Node(Node * current, std::vector<Agent> * agents, std::vector<Box> * boxes)
: parent(current->getParent()), gval(current->g()), action(current->getAction())
{
	this->parent = current->parent;
	this->agents = *agents;
	this->boxes = *boxes;
	agentHash = std::unordered_map<Location, Agent *, LocationHash>();
	boxHash = std::unordered_map<Location, Box *, LocationHash>();
	doHash();
}

bool Node::operator==(const Node * obj) const{
	return (equals(obj));
}

Node & Node::operator=( const Node &first ){
	this->boxes = first.boxes;
	this->agents = first.agents;
	this->parent = first.getParent();
	this->action = first.getAction();
	this->gval = first.g();
	return *this;
}

/*
 * Takes an agent * and a command, and calculates where there must be a moveable box.
 */
Location Node::getBoxLocation(Agent * agent, Command * c){
	if (c->getActionType() == Command::Pull){
		return agent->getLocation() - c->boxdloc();
	} else if (c->getActionType() == Command::Push){
		return agent->getLocation() + c->agentdloc();
	} else {
		throw "getBoxCalled Wrongly\n";
		return Location(-5, -5);
	}
}

/*
Takes an agent, takes a command and checks if it is legal
 */

bool Node::checkState(int agent, Command * c){
	Agent * activeAgent;
	for (int i = 0; i < agents.size(); i++){
		if (agents[i].number == agent){
			activeAgent = &agents[i];
			break;
		}
	}
	//Checks for an agent command that it is legal.
	if (c->getActionType() == Command::Move){
		Location newL = activeAgent->getLocation() + c->agentdloc();
		return this->cellIsFree(newL);
	} else if (c->getActionType() == Command::Pull){
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		if (box == NULL || box->getColor() != activeAgent->getColor())
			return false;
		Location agentL = activeAgent->getLocation() + c->agentdloc();

		return this->cellIsFree(agentL);

	} else if (c->getActionType() == Command::Push){
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		if (box == NULL || box->getColor() != activeAgent->getColor())
			return false;
		boxloc = box->getLocation() + c->boxdloc();
		return this->cellIsFree(boxloc);
	} else {
		return true;
	}
}

/*
Takes an agent, executes a command and checks if it is legal. C
command:
int actionType;
int dirAgent;
int dirBox;
 */
bool Node::checkAndChangeState(int agent, Command * c){

	Agent * activeAgent;
	for (int i = 0; i < agents.size(); i++){
		if (agents[i].number == agent){
			activeAgent = &agents[i];
			break;
		}
	}
	//Checks for legality
	//if (c == )
	if (!checkState(agent, c)){
		return false;
	}
	//Changes state
	if (c->getActionType() == Command::Move){

		activeAgent->setDLocation(c->agentdloc());

	} else if (c->getActionType() == Command::Pull){
		//Calculates box' former position through knowing
		// the new position and command changes from the old.
		//Used for getting the box whose position changes
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);
		box->setLocation(activeAgent->getLocation());

		activeAgent->setDLocation(c->agentdloc());


	} else if (c->getActionType() == Command::Push){
		Location boxloc = getBoxLocation(activeAgent, c);
		Box * box = getBox(boxloc);

		box->setDLocation(c->boxdloc());

		activeAgent->setDLocation(c->agentdloc());

	} else {
		//NoOp, do nothing
	}
	doHash();
	return true;
}

//Initialize static variables:
int Node::maxX;
int Node::maxY;
std::vector<bool> Node::walls;
std::unordered_map<Location, Goal *, LocationHash> Node::goalHash;
//Maybe make it true goals instead of pointers, as this one never needs editing.
std::vector<Goal> Node::goals;

//This should only be used for the first node. Inits walls & goals.

Node::Node() : gval(0) {
	this->parent = NULL;
	this->boxes = std::vector<Box>();
	this->agents = std::vector<Agent>();
	agentHash = std::unordered_map<Location, Agent *, LocationHash>();
	boxHash = std::unordered_map<Location, Box *, LocationHash>();
	doHash();
	action = NULL;
}

Node::Node(Node * parent, Command * c) : gval(parent->g()+1), action(c) {
	this->parent = parent;
	//Inits boxes
	this->agents = (parent->agents);
	this->boxes = (parent->boxes);
	this->action = c;
	agentHash = std::unordered_map<Location, Agent *, LocationHash>();
	boxHash = std::unordered_map<Location, Box *, LocationHash>();
	doHash();
}

int Node::g() const{
	return this->gval;
}

bool Node::isInitialState() {
	return this->parent == NULL;
}

//Used to do the initial hashing of agents and boxes
void Node::doHash(){

	agentHash.clear();
	boxHash.clear();
	for (int i = 0; i < agents.size(); i++){
		agentHash.insert(std::pair<Location, Agent *>(agents[i].getLocation(), (Agent *)&agents[i]));
	}
	for (int i = 0; i < boxes.size(); i++){
		boxHash.insert(std::pair<Location, Box *>(boxes[i].getLocation(), (Box *)&boxes[i]));
	}
}

//Used for rehashing
void Node::deleteAgentFromHash(Agent * a){
	agentHash.erase(a->getLocation());
}

void Node::deleteBoxFromHash(Box * b){
	boxHash.erase(b->getLocation());
}

void Node::insertAgentToHash(Agent * a){
	agentHash.insert(std::pair<Location, Agent *>(a->getLocation(), (Agent *)a));
}

void Node::insertBoxToHash(Box * b){
	boxHash.insert(std::pair<Location, Box *>(b->getLocation(), (Box *)b ));
}

std::vector<Node> Node::getExpandedNodes(char agent)
{

	std::vector<Node> expandedNodes = std::vector<Node>();
	for (const auto & a : agents)
  {
		if (a.getChar() != agent)
			continue;

		Location agentLoc = a.getLocation();
		int coms = Command::EVERY.size();
		for (int i = 0; i < coms; i++)
    {
			Command * c = &(Command::EVERY[i]);
			Location newAgentLoc = a.getLocation() + c->agentdloc();
			if (c->getActionType() == Command::Move)
      {
				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentLoc))
        {
					Node n = Node(this, c);
					Agent * a = n.getAgent(agentLoc);
					n.doHash();
					a->setLocation(newAgentLoc);
					expandedNodes.push_back(n);
				}
			}
      else if (c->getActionType() == Command::Push)
      {
				// Make sure that there's actually a box to move
				if (this->boxAt(newAgentLoc) && this->getBox(newAgentLoc)->getColor() == a.getColor())
        {
					Location newBoxLoc = newAgentLoc + c->boxdloc();
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxLoc))
          {
						Node n = Node(this, c);

						//Setting locations and rehashing
						Agent * a = n.getAgent(agentLoc);
						Box * b = n.getBox(newAgentLoc);
						a->setLocation(newAgentLoc);
						b->setLocation(newBoxLoc);
						n.doHash();

						expandedNodes.push_back(n);
					}
				}
			}
      else if (c->getActionType() == Command::Pull)
      {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentLoc))
        {
					Location boxLoc = agentLoc - c->boxdloc();
					if (this->boxAt(boxLoc) && this->getBox(boxLoc)->getColor() == a.getColor())
          {
						Node n = Node(this, c);

						//Inserting and rehashing
						Agent * a = n.getAgent(agentLoc);
						Box * b = n.getBox(boxLoc);
						a->setLocation(newAgentLoc);
						b->setLocation(agentLoc);
						n.doHash();

						expandedNodes.push_back(n);
					}
				}
			}
		}
	}
	return expandedNodes;
}


// Do not return nodes that destroy the goal
std::vector<Node> Node::getExpandedNodes(char agent, Goal * g)
{
  std::vector<Node> expandedNodes = std::vector<Node>();
	for (const auto & a : agents)
  {
		if (a.getChar() != agent)
			continue;

		Location agentLoc = a.getLocation();
		int coms = Command::EVERY.size();
		for (int i = 0; i < coms; i++)
    {
			Command * c = &(Command::EVERY[i]);
			Location newAgentLoc = a.getLocation() + c->agentdloc();
			if (c->getActionType() == Command::Move)
      {
				// Check if there's a wall or box on the cell to which the agent is moving
				if (this->cellIsFree(newAgentLoc))
        {
					Node n = Node(this, c);
					Agent * a = n.getAgent(agentLoc);
//					n.deleteAgentFromHash(a);
					a->setLocation(newAgentLoc);
//					n.insertAgentToHash(a);
					n.doHash();


					expandedNodes.push_back(n);
				}
			}
      else if (c->getActionType() == Command::Push)
      {
				// Make sure that there's actually a box to move
				if (this->boxAt(newAgentLoc) && this->getBox(newAgentLoc)->getColor() == a.getColor())
        {
					Location newBoxLoc = newAgentLoc + c->boxdloc();
					// .. and that new cell of box is free
					if (this->cellIsFree(newBoxLoc))
          {
						Node n = Node(this, c);
						Agent * a = n.getAgent(agentLoc);
						Box * b = n.getBox(newAgentLoc);
//						n.deleteAgentFromHash(a);
//						n.deleteBoxFromHash(b);
						a->setLocation(newAgentLoc);
						b->setLocation(newBoxLoc);
//						n.insertAgentToHash(a);
//						n.insertBoxToHash(b);
						n.doHash();

            if(n.isGoalState(g))
              expandedNodes.push_back(n);
					}
				}
			}
      else if (c->getActionType() == Command::Pull)
      {
				// Cell is free where agent is going
				if (this->cellIsFree(newAgentLoc))
        {
					Location boxLoc = agentLoc - c->boxdloc();
					if (this->boxAt(boxLoc) && this->getBox(boxLoc)->getColor() == a.getColor())
          {
						Node n = Node(this, c);
						//Inserting and rehashing
						Agent * a = n.getAgent(agentLoc);
						Box * b = n.getBox(boxLoc);
						a->setLocation(newAgentLoc);
						b->setLocation(agentLoc);
						n.doHash();

						if(n.isGoalState(g)){
							expandedNodes.push_back(n);
						}
					}
				}
			}
		}
	}
	return expandedNodes;
}

std::list<Node *> Node::extractPlan()
{
	std::list<Node*> plan = std::list<Node*>();
	Node * n = this;

	while (!(n->isInitialState())) {
		plan.push_front(n);
		n = n->getParent();
	}
	return plan;
}

Node * Node::getParent() const {
	return (Node *) parent;
}
Command * Node::getAction() const {
	return (Command *) action;
}

int Node::hashCode() const
{

	int prime = 31;
	int result = 1;
	for (auto & a : agents){
		result = prime * result + a.getLocation().getX();
		result = prime * result + a.getLocation().getY();
	}
	//Constructs a string and hashes it. Easier than hashing random vectors.
	for (auto & b : boxes){
		result = prime * result + b.getLocation().getX();
		result = prime * result + b.getLocation().getY();
	}
	return result;
}

bool Node::equals(const Node * obj) const {
	if (obj == NULL)
		return false;
	//Using == works with std::vector
	//Assumes the size of agents is the same. Maybe this should be rectified later.
	//This might be an issue. We must ensure that the order of boxes and agents are always identical.
	for (int i = 0; i < agents.size(); i++){
		if (!agents[i].equals(&obj->agents[i])){
			return false;
		}
	}

	//Assumes the size of boxes is the same. Maybe this should be rectified later.
	//This might be an issue. We must ensure that the order of boxes and agents are always identical.
	for (int i = 0; i < boxes.size(); i++){
		if (!boxes[i].equals(&obj->boxes[i])){
			return false;
		}
	}
	return true;
}

std::string Node::toString() {
	std::string s("");
	for (int y = 0; y < maxY; y++) {
		for (int x = 0; x < maxX; x++) {
			if (Node::walls[x+y*maxX]) {
				s += ("+");
			} else {
				s += (" ");
			}
		}
		s.append("\n");
	}

	//Write a goal at location for each goal.
	for (auto & g : goals){
		int x = g.getLocation().getX();
		int y = g.getLocation().getY();
		s[x+y*(maxX+1)] = g.getChar();
	}
	//Write a box at location for each goal.
	for (auto & b : boxes){
		int x = b.getLocation().getX();
		int y = b.getLocation().getY();
		s[x+y*(maxX+1)] = b.getChar();
	}

	for (auto & a : agents){
		int x = a.getLocation().getX();
		int y = a.getLocation().getY();
		s[x+y*(maxX+1)] = a.getChar();
	}

	return s;
}

void Node::hashGoals(){
	for (int i = 0; i < goals.size(); i++){
		goalHash.insert(std::pair<Location, Goal *>(goals[i].getLocation(), (Goal *)&goals[i]));
	}
}

bool Node::isGoalState()
{
	for(auto & goal : Node::goals)
	{
		bool goalState = false;
		for(auto & box : this->boxes)
		{
			if(goal.getLocation() == box.getLocation() && goal.getChar() == std::tolower(box.getChar()))
			{
				goalState = true;
				break;
			}
		}
		if(!goalState){
			return false;
		}
	}
	return true;
}

bool Node::isGoalState(Entity::COLOR color)
{
	for(auto & goal : Node::goals){
		bool goalState = false;
		bool goalHasBox = false;
		for(auto & box : this->boxes) {
			if(box.getColor() != color){
				continue;
			}
			if (std::tolower(goal.getChar()) == std::tolower(box.getChar())){
				goalHasBox = true;
			}

			if(goal.getLocation() == box.getLocation()){
				goalState = true;
				break;
			}
		}
		if (!goalState && goalHasBox){
			//Is Okay
			return false;
		}
	}
	return true;
}

bool Node::isGoalState(Goal * g)
{
	for(auto & b : this->boxes)
	{
		if(g->getLocation() == b.getLocation() && g->getChar() == std::tolower(b.getChar()))
			return true;
	}
	return false;
}

Box * Node::getBox(Location loc)
{
	  std::unordered_map<Location, Box *, LocationHash>::iterator got = boxHash.find(loc);
	  if (got == boxHash.end()){
		  return NULL;
	  } else {
		  return got->second;
	  }
}

Goal * Node::getGoal(Location loc)
{
	  std::unordered_map<Location, Goal *, LocationHash>::iterator got = goalHash.find(loc);
	  if (got == goalHash.end()){
		  return NULL;
	  } else {
		  return got->second;
	  }
}

Agent * Node::getAgent(Location loc)
{
	  std::unordered_map<Location, Agent *, LocationHash>::iterator got = agentHash.find(loc);
	  if (got == agentHash.end()){
		  return NULL;
	  } else {
		  return got->second;
	  }
	//throw new NullPointerException("No agent at row: " + String.valueOf(row) + " and col: " + String.valueOf(col));
}

bool Node::cellIsFree(Location loc)
{
	return !walls[loc.getX() + loc.getY()*maxX] && !boxAt(loc) && !agentAt(loc);
}

bool Node::boxAt(Location loc)
{
	return getBox(loc) != NULL;
}

bool Node::goalAt(Location loc)
{
	return getGoal(loc) != NULL;
}

bool Node::agentAt(Location loc)
{
	return getAgent(loc) != NULL;
}

void Node::solveGoal(Goal * g)
{
  for(auto & b : this->boxes)
	{
		if(std::tolower(g->getChar()) == std::tolower(b.getChar()))
		{
      b.setLocation(g->getLocation());
      return;
		}
	}
}
