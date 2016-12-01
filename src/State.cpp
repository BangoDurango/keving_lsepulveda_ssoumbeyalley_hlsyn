#include "State.h"

State::State()
{
}

State::State(int t)
{
	time = t;
}

void State::addVertex(Vertex * v)
{
	nodes.push_back(v);
}
State* State::combineIfElseStates(State * s1, State * s2, Vertex * vCond)
{/*
	
	std::vector<Vertex*> n1;
	std::vector<Vertex*> n2;
	int time = s1->getTime();
	State* newS = new State(time );
	if (time == s2->getTime ) {
		n1 = s1->getNodes();
		n1.insert()
	}*/
}
std::vector<Vertex*> State::getNodes()
{
	return nodes;
}

int State::getTime()
{
	return time;
}

std::vector<string> State::getVerilog()
{
	return std::vector<string>();
}

void State::setNext(State * n)
{
	nextState = n;
}

std::vector<string> State::getStrings()
{
	std::vector<string> tmpS;

	for (std::vector<Vertex*>::iterator vIt = nodes.begin(); vIt != nodes.end(); ++vIt) {
		tmpS.push_back((*vIt)->getString());
	}

	return ;
}

