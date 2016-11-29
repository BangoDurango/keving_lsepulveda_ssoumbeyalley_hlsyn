#include "Block.h"




//
//Block::Func(std::string sArg, Block * inPrev)
//{
//	arg = sArg;
//	this->fPrev = inPrev;
//	fPrev->setNextFunction(this);
//}
//
//void Block::setElseFunction(Block * inElse)
//{
//	fElse = inElse;
//	fElse->setPrevFunction(this->fPrev);
//}
//
//void Block::setNextFunction(Block * inNext)
//{
//	fNext = inNext;
//}
//
//void Block::setPrevFunction(Block * inPrev)
//{
//	fPrev = inPrev;
//
//}

void Block::addVertex(Vertex * v)
{
	nodes.push_back(v);
}

void Block::addEdge(Edge * e)
{
	edges.push_back(e);
}

void Block::setPrev(Conditional * c)
{
	if (fPrev != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	cPrev = c;
}

void Block::setPrev(Block * f)
{
	if (cPrev != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	fPrev = f;
}

void Block::setNext(Conditional * c)
{
	if (fNext != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	cNext = c;
}

void Block::setNext(Block * f)
{
	if (cNext != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	fNext = f;
}

void Block::clearNext()
{
	this->fNext = NULL;
}

Block* Block::updatePrev(Conditional* c)
{
	Block* tmp;
	tmp = fPrev;
	fPrev->clearNext();
	this->fPrev = NULL;
	this->setPrev(c);
	return tmp;
}



//Conditional * Block::getPrev()
//{
//	return cPrev;
//}
//
//Conditional * Block::getNext()
//{
//	return cNext;
//}
