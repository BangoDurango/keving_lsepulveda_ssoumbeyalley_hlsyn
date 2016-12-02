#include "ControlGraph.h"
ControlGraph::ControlGraph(){
	StartConditional = NULL;
	StartBlock = NULL;
}
void ControlGraph::addBlock(Block * b)
{
	if (blocks.size() == 0){
		StartBlock = b;
	}
	blocks.push_back(b);
	
}

void ControlGraph::addConditional(Conditional * c)
{
	conditionals.push_back(c);
}

Block* ControlGraph::dropLast()
{
	Block* currBlk;
	
	currBlk = blocks.back();

	while (currBlk->getNodes().size() == 0 && currBlk != NULL) {

		blocks.pop_back();
		currBlk = blocks.back();

	}
	blocks.back()->clearNext();

	return blocks.back();
}

std::vector<Block*> ControlGraph::getBlocks()
{
	return blocks;
}

std::vector<Conditional*> ControlGraph::getConditionals()
{
	return conditionals;
}
//std::vector<State*> ControlGraph::getStates(){
//
//
//}
void ControlGraph::setStart(Block* b){
	StartBlock = b;
}
void ControlGraph::setStart(Conditional* c){
	StartConditional = c;
	StartBlock = NULL;
}
//bool ControlGraph::query_isStartConditional(){
//	if (StartConditional != NULL && StartBlock == NULL){
//		return true;
//	}
//	return false;
//}

Conditional* ControlGraph::getStartConditional(){
	return StartConditional;
}
Block*  ControlGraph::getStartBlock(){
	return StartBlock;
}
std::vector<State*> ControlGraph::callGS() {
	std::vector<State*> allStates;
	State::resetStateCount();

	if (StartBlock != NULL) {
		allStates = generateStates(StartBlock);
	}
	else if (StartConditional != NULL) {
		allStates = generateStates(StartConditional);
	}

	objStates = allStates;
	ControlGraph::Bandaid_No1();
	return objStates;
}
void  ControlGraph::Bandaid_No1() {
	std::string s;
	//I AM WRITING THIS PROGRAM ALONE AND I HAVE SPENT WAY TOO MUCH TIME ON IT SO I DON'T CARE IF THIS IS A GOOD SOLUTION. I JUST WANT TO GRADUATE AND NEVER DO A GROUP PROJECT AGAIN UNLESS I'M GETTING PAID.
	for (std::vector<State*>::iterator sIt = objStates.begin() + 1; sIt != objStates.end() - 1;) {
		s = (*sIt)->getNodes().front()->getString();
		if (s == "INPUTS" || s == "OUTPUTS") {
			sIt = objStates.erase(sIt);
		}
		else {
			sIt++;
		}
	}
}
std::vector<State*> ControlGraph::generateStates(Block* b) {
	State* newS;
	Block* currB;
	std::vector<Vertex*> tmpNodes;
	int time;
	std::vector<State*>allStates;
	std::vector<State*>tmpStates;

	State* currS;

	allStates = b->getStates();
	currS = allStates.back();

	if (b->getNextConditional() != NULL) {
		tmpStates = generateStates(b->getNextConditional());
		currS->setNextIfTrue(tmpStates.front());
		allStates.insert(allStates.end(), tmpStates.begin(), tmpStates.end());
		return allStates;
		}
	else if (b->getNextBlock() != NULL) {
		tmpStates = generateStates(b->getNextBlock());
		currS->setNextIfTrue(tmpStates.front());
		allStates.insert(allStates.end(), tmpStates.begin(), tmpStates.end());
		return allStates;
	}
	else {
		return allStates;
	}



}

std::vector<State*> ControlGraph::generateStates(Conditional* c) {
	State* newS;
	Block* currB;
	std::vector<Vertex*> tmpNodes;
	int time;
	std::vector<State*>allStates;
	std::vector<State*>tmpStatesTRUE;
	std::vector<State*>tmpStatesFALSE;
	State* nextS;
	State* currS = c->getConditionalState();
	allStates.push_back(currS);

	if (c->getNextBlk_True() != NULL) {
		tmpStatesTRUE = generateStates(c->getNextBlk_True());
		allStates.insert(allStates.end(), tmpStatesTRUE.begin(), tmpStatesTRUE.end());
		currS->setNextIfTrue(tmpStatesTRUE.front());
	}
	else if (c->getNextCondition_True() != NULL) {
		tmpStatesTRUE = generateStates(c->getNextCondition_True());
		allStates.insert(allStates.end(), tmpStatesTRUE.begin(), tmpStatesTRUE.end());
		currS->setNextIfTrue(tmpStatesTRUE.front());
	}

	if (c->getNextBlk_False() != NULL) {
		nextS = c->getNextBlk_False()->getTopState_if_found();
		if (nextS != NULL) {
			currS->setNextIfFalse(nextS);
		}
		else {
			tmpStatesFALSE = generateStates(c->getNextBlk_False());
			allStates.insert(allStates.end(), tmpStatesFALSE.begin(), tmpStatesFALSE.end());
			currS->setNextIfFalse(tmpStatesFALSE.front());
		}
	}
	else if (c->getNextCondition_False() != NULL) {
		if (c->getNextCondition_False()->checkFound()) {
			currS->setNextIfFalse(c->getNextCondition_False()->getConditionalState());
		}
		else {
			tmpStatesFALSE = generateStates(c->getNextCondition_False());
			allStates.insert(allStates.end(), tmpStatesFALSE.begin(), tmpStatesFALSE.end());
			currS->setNextIfFalse(tmpStatesFALSE.front());
		}
	}

	return allStates;

}
/*for (std::vector<Conditional*>::iterator currC = conditionals.begin(); currC != conditionals.end(); ++currC) {

		newS = (*currC)->getConditionalState();
		time = newS->getTime();
		//////////CONDITIONS
		if ((*currC)->getNextCondition_True() != NULL) {
			

		}
		if ((*currC)->getNextCondition_False() != NULL) {

		}
		/////////////BLOCKS
		if ((*currC)->getNextBlk_True() != NULL) {
		
		}
	
		
		if ((*currC)->getNextBlk_False() != NULL) {
			

		}


		}*/