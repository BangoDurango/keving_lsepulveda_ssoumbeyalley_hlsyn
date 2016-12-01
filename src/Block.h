#ifndef BLOCK_H
#define BLOCK_H

#include "main.h"
#include "Vertex.h"
#include "Edge.h"
//#include "CDFGraph.h"
//#include "Conditional.h"
class Conditional;
class Block {

public:

	Block();
	void addVertex(Vertex* v);
	void addEdge(Edge* e);
	void setPrev(Conditional* c);
	void setPrev(Block* f);
	void setNext(Conditional* c);
	void setNext(Block* blk);
	void clearNext();
	std::vector<Vertex*> getNodes();
	Block* convertToElse(Conditional* c);

	
	//Conditional* getNext();
	Vertex* getFirst();
	void setConverse(Block* blk);
	Block* getConverse();
	void setToElse();
	void setToIf();
	
	bool query_IsElse();
	bool query_IsIf();

	bool checkForVertex(Vertex *v);

	//bool checkForVertexInConverse();
private:

	std::vector<Vertex*> nodes;
	std::vector<Edge*> edges;
	
	bool isElse;
	bool isIf;

	Conditional* cPrev;
	Conditional* cNext;
	Block* blkPrev;
	Block* blkNext;
	Block* BlkConverse;
};

#endif

