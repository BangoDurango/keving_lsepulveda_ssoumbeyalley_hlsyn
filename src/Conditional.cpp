#include "Conditional.h"

Conditional::Conditional() {
blkPrev = NULL;
blkNextIfTrue = NULL;
blkNextIfFalse = NULL;
cndNextIfTrue = NULL;
cndNextIfFalse = NULL;
}
Conditional::Conditional(std::string Arg) {
	blkPrev = NULL;
	blkNextIfTrue = NULL;
	blkNextIfFalse = NULL;
	cndNextIfTrue = NULL;
	cndNextIfFalse = NULL;

	std::vector<string> tok;
	tok = Parser::splitByWhitespace(Arg);
	sArg = tok.at(2);
}
Conditional::Conditional(std::string Arg, Block *inPrev)
{
	blkPrev = NULL;
	blkNextIfTrue = NULL;
	blkNextIfFalse = NULL;
	cndNextIfTrue = NULL;
	cndNextIfFalse = NULL;

	sArg = Arg;
	blkPrev = inPrev;

}

void Conditional::setNextIfTrue(Block *f)
{
	if (cndNextIfTrue != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	blkNextIfTrue = f;
}

void Conditional::setNextIfTrue(Conditional *c)
{
	if (blkNextIfTrue != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	cndNextIfTrue = c;
}

void Conditional::setNextIfFalse(Block * f)
{
//	f->setCParent(this);
	if (cndNextIfFalse != NULL) {
		std::cout << "warning: Conditional already has both block and condition assigned to next." << std::endl;
	}
	blkNextIfFalse = f;
}

void Conditional::setNextIfFalse(Conditional * c)
{
	if (blkNextIfFalse != NULL) {
		std::cout << "warning: Conditional already has both block and condition assigned to next." << std::endl;
	}
	cndNextIfFalse = c;
}

Block * Conditional::getNextBlk_True()
{
	return blkNextIfTrue;
}

Block * Conditional::getNextBlk_False()
{
	return blkNextIfFalse;
}

Conditional * Conditional::getNextCondition_True()
{
	return cndNextIfTrue;
}

Conditional * Conditional::getNextCondition_False()
{
	return cndNextIfFalse;
}

void Conditional::setVCondition(Vertex * v)
{
	vCondition = v;
}



Vertex * Conditional::getVCondition()
{
	return vCondition;
}

std::string Conditional::getSCondition()
{
	return sArg;
}

std::vector<Edge*> Conditional::connectVCnd(){

	std::vector<Vertex*> vVec;
	std::vector<Edge*> eVec;
	Edge* newE;
	//for (std::vector<Vertex*>::iterator it = )
	if (blkNextIfTrue != NULL){
		vVec = blkNextIfTrue->getNodes();
		for (std::vector<Vertex*>::iterator it = vVec.begin(); it != vVec.end(); ++it) {
			newE = new Edge(VARIABLE, this->sArg);
			this->vCondition->addOutgoing(newE);
			(*it)->addIncoming(newE);
			
			eVec.push_back(newE);
		}
	}
	else if (cndNextIfTrue != NULL){
		newE = new Edge(VARIABLE, this->sArg);
		cndNextIfTrue->getVCondition()->addIncoming(newE);
		vCondition->addOutgoing(newE);
		eVec.push_back(newE);
	}
	
	/////////////////////////////////////////////
	if (blkNextIfFalse != NULL && blkNextIfFalse->query_IsElse() == true) {
		vVec = blkNextIfFalse->getNodes();
		for (std::vector<Vertex*>::iterator it = vVec.begin(); it != vVec.end(); ++it) {
			newE = new Edge(VARIABLE, this->sArg);
			this->vCondition->addOutgoing(newE);
			(*it)->addIncoming(newE);
			eVec.push_back(newE);
		}
	}
	
	return eVec;
}