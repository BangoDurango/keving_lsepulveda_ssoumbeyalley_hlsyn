#pragma once
#ifndef CDFGRAPH_H
#define CDFGRAPH_H

#include "main.h"
#include "Parser.h"


#include "IOV.h"
#include "Edge.h"
#include "Vertex.h"

#include "ControlGraph.h"
//class Vertex;
//class Edge;

class CDFGraph {

public:

	CDFGraph();

	void setLatency(int n);
	int getLatency();
	void loadFileStrings(std::vector<string> strVec);
	void loadIOV(std::vector<IOV> ins, std::vector<IOV> outs, std::vector<IOV> vars);
	IOV *getIOVbyName(std::string s);
	void parseOperation(string s);
	void parseInput(string s, Vertex* newV);
	void parseOutput(string s, Vertex* newV);
	//void parseIF(string s, Conditional *c);
	void printGraph();
	void parseOperations();
	void addConditionalVertices();
	Vertex* parseConditional(string s);
	//Conditional* parseConditional(string s);
	std::vector<Edge*> getEdgesByID(string s);
	std::vector<Vertex*> getVertices();
	void DFS(CDFGraph* g, Vertex* v);
	void ALAP(CDFGraph* g, Vertex* v, int time);
	void ALAP(CDFGraph * g, int n);
	void ALAP(int n);

	void LIST_R(int n);

	void LIST_R(CDFGraph * g, Vertex * v);

private:
	std::vector<IOV> inputs;
	std::vector<IOV> outputs;
	std::vector<IOV> variables;
	Vertex* vINOP;
	Vertex* vONOP;

	std::vector<Vertex*> Vertices;
	std::vector<Edge*> Edges;
	std::vector<string> FileStrings;

	//int AddSubCnt;
	//int MultCnt;
	//int LogicCnt;
	//int IfCnt;

	ControlGraph gControlGraph;
	Block* currBlk;
	Block* StartBlock;
	Conditional* currC;
	std::vector<Conditional*> CondVec;
	BlockType _last;

	Resource ADDER_SUBTRACTOR;
	Resource MULTIPLIER;
	Resource LOGICAL;
	Resource IF_STATEMENT;
	int latency;
};


#endif