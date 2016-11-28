#include "Func.h"




//
//Func::Func(std::string sArg, Func * inPrev)
//{
//	arg = sArg;
//	this->fPrev = inPrev;
//	fPrev->setNextFunction(this);
//}
//
//void Func::setElseFunction(Func * inElse)
//{
//	fElse = inElse;
//	fElse->setPrevFunction(this->fPrev);
//}
//
//void Func::setNextFunction(Func * inNext)
//{
//	fNext = inNext;
//}
//
//void Func::setPrevFunction(Func * inPrev)
//{
//	fPrev = inPrev;
//
//}

void Func::addVertex(Vertex * v)
{
	nodes.push_back(v);
}

void Func::addEdge(Edge * e)
{
	edges.push_back(e);
}

void Func::setPrev(Conditional * c)
{
	if (fPrev != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	cPrev = c;
}

void Func::setPrev(Func * f)
{
	if (cPrev != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	fPrev = f;
}

void Func::setNext(Conditional * c)
{
	if (fNext != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	cNext = c;
}

void Func::setNext(Func * f)
{
	if (cNext != NULL) {
		std::cout << "warning: Function already has both function and condition assigned to next." << std::endl;
	}
	fNext = f;
}

void Func::clearNext()
{
	this->fNext = NULL;
}

Func* Func::updatePrev(Conditional* c)
{
	Func* tmp;
	tmp = fPrev;
	fPrev->clearNext();
	this->fPrev = NULL;
	this->setPrev(c);
	return tmp;
}



//Conditional * Func::getPrev()
//{
//	return cPrev;
//}
//
//Conditional * Func::getNext()
//{
//	return cNext;
//}
