#pragma once
#ifndef STATE_H
#define STATE_H

#include "main.h"
#include "Vertex.h"

class State {

public:
	State();
	State(int t);
	void addVertex(Vertex* v);
	static State* combineIfElseStates(State* s1, State* s2, Vertex* vCond);
	std::vector<Vertex*> getNodes();
	int getTime();
	std::vector<string> getVerilog();
	void setNext(State* n);
	std::vector<string> getStrings();

private:

	State* nextState;
	int time;
	std::vector<Vertex*> nodes;

};
#endif