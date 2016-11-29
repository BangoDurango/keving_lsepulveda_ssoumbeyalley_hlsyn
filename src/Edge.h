#pragma once
#ifndef EDGE_H
#define EDGE_H

#include "main.h"
//#include "CDFGraph.h"
#include "IOV.h"
class Vertex;
class Edge {

public:
	Edge(std::string strType, std::string strID);
	Edge(Vertex *v1, Vertex *v2);
	void setOutput(Vertex *v);

	Vertex* getOutput();
	void setInput(Vertex *v);
	Vertex* getInput();
	void setID(string s);
	std::string getID();
	void printEdge();
private:
	std::string sType;
	std::string ID;
	Vertex *out;
	Vertex *in;


	IOV var;
};
#endif