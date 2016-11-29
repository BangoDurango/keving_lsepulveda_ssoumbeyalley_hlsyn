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

Block::Block()
{
	cPrev = NULL;
	cNext = NULL;
	blkPrev = NULL;
	blkNext = NULL;
}

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
	if (blkPrev != NULL) {
		std::cout << "warning: Block already has both function and condition assigned to next." << std::endl;
	}
	cPrev = c;
}

void Block::setPrev(Block * f)
{
	if (cPrev != NULL) {
		std::cout << "warning: Block already has both function and condition assigned to next." << std::endl;
	}
	blkPrev = f;
}

void Block::setNext(Conditional * c)
{
	if (blkNext != NULL) {
		std::cout << "warning: Block already has both function and condition assigned to next." << std::endl;
	}
	cNext = c;
}

void Block::setNext(Block * f)
{
	if (cNext != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	blkNext = f;
}

void Block::clearNext()
{
	this->blkNext = NULL;
}

std::vector<Vertex*> Block::getNodes()
{
	return nodes;
}

Block* Block::updatePrev(Conditional* c)
{
	Block* tmp;
	tmp = blkPrev;
	blkPrev->clearNext();
	this->blkPrev = NULL;
	this->setPrev(c);
	return tmp;
}

Vertex * Block::getFirst()
{
	return nodes.front();
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
