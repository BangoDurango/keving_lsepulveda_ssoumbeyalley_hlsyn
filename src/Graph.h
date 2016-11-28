#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "main.h"
#include "Edge.h"
#include "Vertex.h"
#include "Parser.h"
#include "Func.h"
#include "CDFG.h"


class Graph {

public:

	Graph();
	void loadFileStrings(std::vector<string> strVec);
	void loadIOV(std::vector<IOV> ins, std::vector<IOV> outs, std::vector<IOV> vars);
	IOV *getIOVbyName(std::string s);
	void parseOperation(string s);
	void parseInput(string s, Vertex* newV);
	void parseOutput(string s, Vertex* newV);
	void parseConditional(string s);
	void printGraph();
	void parseOperations();
	//Conditional* parseConditional(string s);
	std::vector<Edge*> getEdgesByID(string s);
	std::vector<Vertex*> getVerticesByOutput(string s);
private:
	std::vector<IOV> inputs;
	std::vector<IOV> outputs; 
	std::vector<IOV> variables;
	Vertex* vINOP;
	Vertex* vNOP;
	//std::vector<Edge*> vINOP;
	//std::vector<Edge*> vNOP;
	//std::vector<Edge*> vVAR;

	std::vector<Vertex*> Vertices;
	std::vector<Edge*> Edges;
	std::vector<string> FileStrings;

	int AddSubCnt;
	int MultCnt;
	int LogicCnt;
	int IfCnt;
	//Conditional* start;
	//std::vector<Conditional*> chunks;
	//Edge* lastCond;
	//std::vector<Edge*> conditionals;

	//bool cFlag;
};
#endif