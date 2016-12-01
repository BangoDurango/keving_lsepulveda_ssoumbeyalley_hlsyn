#include "Block.h"


Block::Block()
{	
	cParent = NULL;
	//vCond = NULL;
	BlkConverse = NULL;
	cPrev = NULL;
	cNext = NULL;
	blkPrev = NULL;
	blkNext = NULL;
	isIf = false;
	isElse = false;
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

Block* Block::convertToElse(Conditional* c)
{	
	isElse = true;
	Block* tmp;
	tmp = blkPrev;
	this->BlkConverse = tmp;
	tmp->setConverse(this);
	blkPrev->clearNext();
	this->blkPrev = NULL;
	this->setPrev(c);
	return tmp;
}

Vertex * Block::getFirst()
{
	return nodes.front();
}

void Block::setConverse(Block * blk)
{
	BlkConverse = blk;
}

Block * Block::getConverse()
{
	return BlkConverse;
}

void Block::setToElse()
{
	if (isElse) {
		std::cout << "ERROR THIS BLOCK IS AN ELSE AND AN IF WHAT GIVES(ELSE)" << std::endl;
	}
	isElse = true;
}

void Block::setToIf()
{
	if (isElse) {
		std::cout << "ERROR THIS BLOCK IS AN ELSE AND AN IF WHAT GIVES (IF)" << std::endl;
	}
	isIf = true;

}

bool Block::query_IsElse()
{
	
	return isElse;
}

bool Block::query_IsIf()
{
	return isIf;
}

bool Block::checkForVertex(Vertex* v) {

	
		if (std::find(nodes.begin(), nodes.end(), v) != nodes.end()) {
			return true;
		}
		return false;
}
bool sortbySchedule2(Vertex *lhs, Vertex *rhs)
{
	bool val;
	val = false;

	if (lhs->query_Schedule() < rhs->query_Schedule()) {
		val = true;
	}

	return val;

}
std::vector<State*> Block::getStates() {
	State* currS;
	int tCurr, tLast;
	std::vector<State*> states;
	std::vector<Vertex*> elseNodes;
	Vertex* condV;

	std::sort(nodes.begin(), nodes.end(), sortbySchedule2);
	tLast = nodes.front()->query_Schedule();
	currS = new State(tLast);



	states.push_back(currS);

	if (!isIf && !isElse) {

		for (std::vector<Vertex*>::iterator currV = nodes.begin(); currV != nodes.end(); ++currV) {
			tCurr = (*currV)->query_Schedule();
			if (tCurr == currS->getTime()) {
				currS->addVertex(*currV);
			}
			else {
				tCurr == currS->getTime();
				currS = new State(tCurr);
				currS->addVertex(*currV);
				states.push_back(currS);
			}
		}
	}
	
	return states;
}

void Block::setCParent(Conditional * c)
{
	//cParent = c;
}

Conditional * Block::getCParent()
{
	return cPrev;
}

