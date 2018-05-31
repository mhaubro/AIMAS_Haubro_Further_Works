#include "CentralPlanner.h"
#include "HandleGoalTask.h"
#include <iostream>
#include <stack>
#include <list>
#include <algorithm>
#include <ctime>
#include <cstdlib>

std::vector<int> CentralPlanner::allPairsShortestPaths;

void CentralPlanner::computeAllPairsShortestPaths(Node * n){
	allPairsShortestPaths = std::vector<int>(Node::maxX*Node::maxX*Node::maxY*Node::maxY);
	for (int x = 0; x <Node::maxX; x++){
		for (int y = 0; y < Node::maxY; y++){
			Location Thisloc = Location(x, y);
			Location currloc = Thisloc;

			//allPairsShortestPaths[Thisloc.getIndex()] = std::vector<int>(Node::maxX*Node::maxY);
			//Theres a wall at current location
			if (Node::walls[Thisloc.getIndex()]){
				continue;
			}

			std::vector<bool> visited = Node::walls;
			//Initializing all values to something high
			//Sets base location to 0

			allPairsShortestPaths[currloc.getIndex()+currloc.getIndex()*Node::maxX*Node::maxY] = 0;
			visited[Thisloc.getIndex()] = true;
			std::list<Location> q = std::list<Location>();

			q.push_back(Thisloc);

			//Do a BFS
			std::vector<Location> Locations;
			while (!q.empty()){
				Thisloc = q.front();
				q.pop_front();
				Locations.clear();
				Locations.push_back(Thisloc+Location(1,0));
				Locations.push_back(Thisloc+Location(-1,0));
				Locations.push_back(Thisloc+Location(0,1));
				Locations.push_back(Thisloc+Location(0,-1));

				for (Location loc : Locations){
					if (loc.isOutOfBounds() || visited[loc.getIndex()]){
						continue;
					}
					visited[loc.getIndex()] = true;
					//A child is 1 more than its parent
					allPairsShortestPaths[loc.getIndex()+currloc.getIndex()*Node::maxX*Node::maxY] = allPairsShortestPaths[Thisloc.getIndex()+currloc.getIndex()*Node::maxX*Node::maxY] + 1;
					q.push_back(loc);
				}
			}
		}
	}
}

CentralPlanner::CentralPlanner(int region){
	this->region = region;
	UnassignedGoals = std::vector<HandleGoalTask *>();
	compatibleGoals = std::vector<std::vector<bool>>(Entity::NUMCOLS);
}

void CentralPlanner::preAnalyse(Node * n){
	computeAllPairsShortestPaths(n);
	getCompatibleGoals(n);
	DetectTasks(n);
	setPredecessors(n);
}

bool CentralPlanner::isGoalCompatible(int goal, Entity::COLOR color){
	return compatibleGoals[goal][color];
}

void CentralPlanner::getCompatibleGoals(Node * n){
	//This function should preferably just calculate all goals, as we don't care about overhead, and accessing the
	//Goals number in here seems to be pretty convenient.
	//Colors assumed to be the same as in entity. Enums are KNOWN to start at zero.
	compatibleGoals.resize(n->goals.size());
	for (int i = 0; i < n->goals.size(); i++){
		compatibleGoals[i] = std::vector<bool>(8);
		for (int j = 0; j < n->boxes.size(); j++){
			//Going through all boxes, storing if the color matches.
			if (tolower(n->boxes[j].getChar()) == n->goals[i].getChar()){
				compatibleGoals[i][n->boxes[j].getColor()] = true;
			}
		}
	}
}

bool CentralPlanner::isFree(Node * n, Location gLoc)
{
		int west = (int) n->walls[gLoc.getX()-1 + gLoc.getY()*n->maxX];
		int north = (int) n->walls[gLoc.getX() + (gLoc.getY()-1)*n->maxX];
		int east = (int) n->walls[gLoc.getX()+1 + gLoc.getY()*n->maxX];
		int south = (int) n->walls[gLoc.getX()+(gLoc.getY()+1)*n->maxX];
		int noWalls = west+north+east+south;
		return noWalls < 2;
}

void CentralPlanner::setPredecessors(Node * n)
{

for (int i = 0; i < n->goals.size(); i++){
	for (int j = 0; j < n->goals.size(); j++){
		bool boxFound = false;
		bool agentFound = false;

		if (j==i){
			continue;
		}

		Location Thisloc = n->goals[i].getLocation();
		Location currloc = n->goals[j].getLocation();

		std::vector<bool> visited = Node::walls;
		visited[currloc.getIndex()] = true;
		visited[Thisloc.getIndex()] = true;

		//Initializing all values to something high
		//Sets base location to 0

		std::list<Location> q = std::list<Location>();

		q.push_back(Thisloc);

		//Do a BFS
		std::vector<Location> Locations;
		while (!q.empty()){
			Thisloc = q.front();
			q.pop_front();
			Locations.clear();
			Locations.push_back(Thisloc+Location(1,0));
			Locations.push_back(Thisloc+Location(-1,0));
			Locations.push_back(Thisloc+Location(0,1));
			Locations.push_back(Thisloc+Location(0,-1));

			for (Location loc : Locations){
				if (loc.isOutOfBounds() || visited[loc.getIndex()]){
					continue;
				}
				visited[loc.getIndex()] = true;
				//A child is 1 more than its parent
				if (Box * b = n->getBox(loc)){
					//It is solvable
					if (std::tolower(b->getChar()) == n->goals[i].getChar()){
						boxFound = true;
						break;
					}
				}
				q.push_back(loc);
			}
			if (boxFound){
				break;
			}
		}
		if (!boxFound){
			 setPredecessor(&n->goals[i], &n->goals[j]);
		}
	}
}


}

void CentralPlanner::setPredecessor(Goal * g1, Goal * g2)
{
  g2->predecessors.push_back(g1);
  // Find its task and set it
  for(int k = 0; k < UnassignedGoals.size(); k++)
  {
    // Only need to look at the task fitting the goal
    if(UnassignedGoals[k]->chr != g1->getChar())
      continue;

    for(int l = 0; l < UnassignedGoals.size(); l++)
    {
      // Only need to look at the task fitting the goal
      if(k == l || UnassignedGoals[l]->chr != g2->getChar())
        continue;

      if(UnassignedGoals[k]->destination == g1->getLocation() && UnassignedGoals[l]->destination == g2->getLocation())
      {
        UnassignedGoals[l]->predecessors.push_back(UnassignedGoals[k]);
        return;
      }
    }
  }
}

// Checks if g1 needs to be solved before g2
bool CentralPlanner::getOrderOfGoals(Node * n, Goal * g1, Goal * g2)
{
/*  for (Goal * predecessor : g1->predecessors){
	  for (Goal * potentialpredecessors : predecessor->predecessors){

	  }
  }*/
  Node * state = Node::getopCopy(n);

  state->solveGoal(g2);

  if(state == nullptr)
    return false;

  // Can g2 be solved?
  if(state->isGoalState(g2))
  {
    state = FindSolution(state, g1, g2);

    // If we cannot solve g1 after having solved g2,
    // then that means g1 needs to be solved first
    if(state == nullptr)
      return true;
  }
  return false;
}


int myrandom (int i) { return std::rand()%i;}

bool CentralPlanner::hasJob(Agent * agent, Node * state){
	std::srand ( unsigned ( std::time(0) ) );
	for (HandleGoalTask * h : UnassignedGoals){
		if (h->solvingColors[agent->getColor()] && !h->seemsCompleted(agent, state))
			return true;
	}

	std::random_shuffle ( freeSpaceTasks.begin(), freeSpaceTasks.end(), myrandom);

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state))
			return true;
	}

	return false;
}

Task * CentralPlanner::getJob(Agent * agent, Node * state){
	//The agent will be the only one to get this task
	//For all boxes and goals, find the one with lowest h-value.

	double hvalTask = 10000000000.0;
	HandleGoalTask * bestTask = NULL;
	int bestIt = 0;
	int counter = 0;

	for (int i = 0; i < UnassignedGoals.size(); i++){
		if(!UnassignedGoals[i]->predecessorsComplete(agent, state)){
			counter++;
			continue;
		}else{
		}
		Box * bestBox = nullptr;
		HandleGoalTask * h = UnassignedGoals[i];


		if (h->seemsCompleted(agent, state))
			continue;

		if (h->solvingColors[agent->getColor()]){
			//Deletes the move box thing
			//Finds most fitting box, by going through all setting and calculating h-val
			double hval = 10000000000000.0;
			for (auto &b : state->boxes){
				if ((tolower(b.getChar()) != h->chr) || b.workInProgress || b.getColor() != agent->getColor()){
					continue;
				}
				h->box = &b;
				double boxh = h->h(agent, state);
				Goal * g = state->getGoal(b.getLocation());
				// Check if box is on a goal already. If it is only take it with some probability.
				if((g == NULL) || (g->getChar() != tolower(b.getChar()))){
					if (boxh < hval){
						hval = boxh;
						bestBox = &b;
					}
				}
			}
			if (bestBox == nullptr){
				continue;
			}
			h->box = bestBox;
			if(hval < hvalTask){
				bestTask = h;
				bestIt = i;
				hvalTask = hval;
			}
		}
	}

	if(bestTask != NULL){
		UnassignedGoals.erase(UnassignedGoals.begin()+bestIt);
		bestTask->box->workInProgress = true;
		return bestTask;
	}

	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state)){
			return t;
		}
	}

	return NULL;
}

bool CentralPlanner::addRequestFreeSpaceTask(RequestFreeSpaceTask * h){
	if (h == NULL)
		return false;
	freeSpaceTasks.push_back(h);
	return true;
}

//Returns a goal task from an agent to the planner
bool CentralPlanner::returnGoalTask(HandleGoalTask * h){
	if (h != NULL){
		// Add value to downprioritise if it can not be solved.
		h->startHval += 50;
		this->UnassignedGoals.push_back(h);
		h->box->workInProgress = false;
		return true;
	}
	return false;
}

bool CentralPlanner::removeRequestTask(RequestFreeSpaceTask * h){
	if (h == NULL)
		return true;
	for (int i = 0; i < freeSpaceTasks.size(); i++){
		if (freeSpaceTasks[i] == h){//Yes, a pointer match
			freeSpaceTasks.erase(freeSpaceTasks.begin()+i);
		}
	}
	for (int i = 0; i < node->agents.size(); i++){
		node->agents[i].removeFreeSpaceTaskMessage(h);
	}
	return true;
}

void CentralPlanner::DetectTasks(Node * n)
{
	for(int i = 0; i < n->goals.size(); i++)
	{
		Goal * g = &n->goals[i];
		if (g->getRegion() != region)
			continue;
		HandleGoalTask * t = new HandleGoalTask(g->getLocation(), 100, compatibleGoals[i], g->getChar());
		UnassignedGoals.push_back(t);
	}
	//throw "Crash\n"
}

bool CentralPlanner::stillActiveRequest(RequestFreeSpaceTask * h){
	if (h == NULL)
		return false;
	for (int i = 0; i < freeSpaceTasks.size(); i++){
		if (freeSpaceTasks[i] == h){//Yes, a pointer match
			return true;
		}
	}
	return false;
}

Node * CentralPlanner::FindSolution(Node * n, Goal * g1, Goal * g2)
{
	for(auto & b : n->boxes)
	{
		if(std::tolower(g1->getChar()) == std::tolower(b.getChar()))
		{
			HandleGoalTask * t = new HandleGoalTask(g1->getLocation(), 0, &b);
			// Find agent to solve task
			for(auto & a : n->agents)
			{
				if(a.getColor() == t->box->getColor())
				{
					a.task = t;
					std::list<Node*> solution = a_star_search(n, &a, a.task, g1, g2);
					a.task = nullptr;
					return solution.back();
				}
			}
		}
	}
	return nullptr;
}

RequestFreeSpaceTask * CentralPlanner::getHelpJob(Agent * agent, Node * state){
	for (RequestFreeSpaceTask * t : freeSpaceTasks){
		if (!t->seemsCompleted(agent, state)){
			return t;
		}
	}
	return NULL;
}

bool CentralPlanner::hasHelpJob(Agent * agent, Node * state){
	return (NULL != getHelpJob(agent, state));
}
