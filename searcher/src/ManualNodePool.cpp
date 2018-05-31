/*
 * ManualNodePool.cpp
 *
 *  Created on: 14 May 2018
 *      Author: martin
 */

#include "ManualNodePool.h"
#include <iostream>

std::vector<Node> ManualNodePool::nodes = std::vector<Node>();

ManualNodePool pool = ManualNodePool();

ManualNodePool::ManualNodePool() {

}

void ManualNodePool::setup(int memInMB) {
	//Raw nodes fit half the memory limit. Should only be done once.
	currentNode = 0;
	numberOfNodes = memInMB*1024*1024/sizeof(Node)/2;
	nodes.resize(numberOfNodes);
}


ManualNodePool::~ManualNodePool() {
	// TODO Auto-generated destructor stub
}

Node * ManualNodePool::createNodeCopy(Node * n, std::vector<Agent> a, std::vector<Box> b){
	//Out of memory
	if (numberOfNodes <= currentNode - 1){
		return NULL;
	}

	nodes[currentNode] = Node(n, &n->agents, &n->boxes);
	currentNode++;
	return &nodes[currentNode-1];
}

void ManualNodePool::clearNodes(){
	currentNode = 0;
}

int ManualNodePool::getUsedNodes(){
	return numberOfNodes - currentNode + 1;
}

double ManualNodePool::getUsedPercentage(){
	return ((double)numberOfNodes)/((double) currentNode + 1.0);
}

