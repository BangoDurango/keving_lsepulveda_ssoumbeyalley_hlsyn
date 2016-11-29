#ifndef BLOCK_H
#define BLOCK_H

#include "main.h"
#include "Vertex.h"
#include "Edge.h"
//#include "CDFGraph.h"
#include "Conditional.h"

class Block {

public:

	void addVertex(Vertex* v);
	void addEdge(Edge* e);
	void setPrev(Conditional* c);
	void setPrev(Block* f);
	void setNext(Conditional* c);
	void setNext(Block* f);
	void clearNext();
	Block* updatePrev(Conditional* c);
	//Conditional* getPrev();
	//Conditional* getNext();
private:

	std::vector<Vertex*> nodes;
	std::vector<Edge*> edges;
	
	Conditional* cPrev;
	Conditional* cNext;
	Block* fPrev;
	Block* fNext;
};
#endif