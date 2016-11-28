#pragma once

#ifndef VERTEX_H
#define VERTEX_H

#include "main.h"
#include "Edge.h"
//class Edge;
class Vertex {

public:
	Vertex();
	Vertex(int n);
	Vertex(int n, std::string strType);
	static std::string checkValidOp(std::string s);
	void setString(std::string s);
	void setID(int n);
	int getID();
	void setType(string s);
	std::string getType();
	//void setE1E2(Edge* e1, Edge* e2);
	//void setE0(Edge *out);
	//Edge* getE0();
	void printVertex();

	void addIncoming(Edge* e);
	void addOutgoing(Edge* e);
	void setIfTrue(Edge* e);
	void setElse(Edge* e);

private:
	std::string sType;
	int ID;

	std::vector<Edge*> incoming;
	std::vector<Edge*> outgoing;
	bool isCond;
	Edge* ifTrue;
	Edge* Else;
	std::string str;


};
#endif