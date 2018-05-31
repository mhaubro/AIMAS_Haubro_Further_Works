/*
 * ManualNodePool.h
 *
 *  Created on: 14 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_MANUALNODEPOOL_H_
#define SEARCHER_SRC_MANUALNODEPOOL_H_

#include "Node.h"
#include <vector>
class Node;

//Might do 1 pr. agent? Might not make sense.
//Fits a fixed amount of nodes
class ManualNodePool {


public:
	int numberOfNodes;
	ManualNodePool();
	virtual ~ManualNodePool();
	void setup(int memInMB);
	Node * createNodeCopy(Node * n, std::vector<Agent> agents, std::vector<Box> boxes);
	void clearNodes();
	int getUsedNodes();
	double getUsedPercentage();


private:
	int currentNode;
	static std::vector<Node> nodes;

};

extern ManualNodePool pool;


#endif /* SEARCHER_SRC_MANUALNODEPOOL_H_ */
