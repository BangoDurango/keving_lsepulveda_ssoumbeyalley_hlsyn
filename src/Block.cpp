#include "Block.h"


Block::Block()
{	
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
//
//bool Block::checkForVertexInConverse()
//{	
//	std::vector<Edge*> eVec;
//	std::vector<Vertex*> vVec;
//	Vertex* vTMP1;
////	Vertex* vTMP2;
//
//	if (isIf || isElse) {
//
//		//vVec = BlkConverse->getNodes();
//
//		for (std::vector<Vertex*>::iterator vIt = nodes.begin(); vIt != nodes.end(); ++vIt) {
//			//for all of this blocks nodes....
//			vTMP1 = (*vIt);
//
//			eVec = vTMP1->getIncoming();//if this guy has any edges leading to the converse block
//			for (std::vector<Edge*>::iterator eIt = eVec.begin(); eIt != eVec.end(); ++eIt) {
//				if (BlkConverse->checkForVertex((*eIt)->getInput())) {
//					(*vIt)->outgoing.erase(eIt);
//
//					edges.erase(eIt);
//				}
//
//			}
//		}
//
//	}
//	return false;
//}





//Conditional * Block::getPrev()
//{
//	return cPrev;
//}
//
//Conditional * Block::getNext()
//{
//	return cNext;
//}
