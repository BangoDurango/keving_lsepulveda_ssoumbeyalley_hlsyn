#ifndef FUNC_H
#define FUNC_H

#include "main.h"
#include "Vertex.h"
#include "Edge.h"
#include "Conditional.h"
class Func {

public:

	void addVertex(Vertex* v);
	void addEdge(Edge* e);
	void setPrev(Conditional* c);
	void setPrev(Func* f);
	void setNext(Conditional* c);
	void setNext(Func* f);
	void clearNext();
	Func* updatePrev(Conditional* c);
	//Conditional* getPrev();
	//Conditional* getNext();
private:

	std::vector<Vertex*> nodes;
	std::vector<Edge*> edges;
	
	Conditional* cPrev;
	Conditional* cNext;
	Func* fPrev;
	Func* fNext;
};
#endif