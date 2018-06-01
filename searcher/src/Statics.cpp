/*
 * Statics.cpp
 *
 *  Created on: 31 May 2018
 *      Author: martin
 */

#include <vector>
#include <list>
#include <unordered_map>

#include "Statics.h"
#include "Location.h"
#include "Goal.h"
#include "Base.h"
#include "Actor.h"


using std::vector;
using std::list;
using std::pair;
using std::unordered_map;

Statics * statics;

Statics::Statics(unordered_map<Location, Goal, LocationHash> goals, vector<Base> bases, vector<bool> walls, int maxX, int maxY, StateRepresentation startstate) :
maxX(maxX), maxY(maxY), goals(goals), bases(bases), fixedState(startstate),
walls(walls), runningState(startstate), actors(computeActors()),
allPairsShortestPath(computeAllPairsShortestPath())

{
	Location::maxX = maxX;
	Location::maxY = maxY;
}

/*
 * Sets up actors for a map
 */

vector<Actor> Statics::computeActors(){
	vector<Actor> tempActors;

	for (auto it = fixedState.agents.begin(); it != fixedState.agents.end(); ++it){
		tempActors.emplace_back(it->second);
	}
	return tempActors;
}

/*
 * Computes all pairs shortest path for a map.
 */
vector<int> Statics::computeAllPairsShortestPath(){
	vector<int> storage = vector<int>(maxX*maxX*maxY*maxY);
	for (int x = 0; x < maxX; x++){
		for (int y = 0; y < maxY; y++){
			Location Thisloc = Location(x, y);
			Location currloc = Thisloc;

			//Theres a wall at current location
			if (walls[Thisloc.getIndex()]){
				continue;
			}

			std::vector<bool> visited = walls;
			//Initializing all values to something high
			//Sets base location to 0

			storage[currloc.getIndex()+currloc.getIndex()*maxX*maxY] = 0;
			visited[Thisloc.getIndex()] = true;
			list<Location> q = list<Location>();

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
					storage[loc.getIndex()+currloc.getIndex()*maxX*maxY] = storage[Thisloc.getIndex()+currloc.getIndex()*maxX*maxY] + 1;
					q.push_back(loc);
				}
			}
		}
	}
	return storage;
}

Statics::~Statics() {
	// TODO Auto-generated destructor stub
}

