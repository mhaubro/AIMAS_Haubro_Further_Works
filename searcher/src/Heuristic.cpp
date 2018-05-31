#include "Heuristic.h"
#include <locale>
#include <string>
#include <sstream>
#define INTEGER_MAX 0x7FFFFFFF//31 1-bits is max due to sign.

Heuristic::~Heuristic(){

}

Heuristic::Heuristic(Node * initialState)
{

}

int Heuristic::h(Node * n)
{
	return 0;
}

int Heuristic::f(Node * n){
	return 0;
}




AStar::AStar(Node * initialState) : Heuristic(initialState) {
}


int AStar::f(Node * n) {
	return n->g() + this->h(n);
}


std::string AStar::toString() {
	return "A* evaluation";
}





WeightedAStar::WeightedAStar(Node *initialState, int W) : Heuristic(initialState){
	this->W = W;
}


int WeightedAStar::f(Node *n) {
	return n->g() + this->W * this->h(n);
}


std::string WeightedAStar::toString() {
	std::ostringstream os;
	os << "WA*(" << this->W <<") evaluation\n";
	return os.str();
}




Greedy::Greedy(Node * initialState) : Heuristic(initialState){
}


int Greedy::f(Node * n) {
	return this->h(n);
}


std::string Greedy::toString() {
	return "Greedy evaluation";
}
