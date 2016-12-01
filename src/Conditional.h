#pragma once
#ifndef IF_H
#define IF_H

#include "main.h"
#include "Vertex.h"
#include "Parser.h"
#include "Block.h"
//class Block;
class Conditional {

public:
	Conditional();
	Conditional(std::string Arg);
	Conditional(std::string s, Block* fPrev);
	void setNextIfTrue(Block* blk);
	void setNextIfTrue(Conditional *c);
	void setNextIfFalse(Block* blk);
	void setNextIfFalse(Conditional *c);
	Block* getNextBlk_True();
	Block* getNextBlk_False();
	Conditional* getNextCondition_True();
	Conditional* getNextCondition_False();
	void setVCondition(Vertex* v);
	
	Vertex* getVCondition();
	std::vector<Edge*> connectVCnd();
	std::string getSCondition();
	
private:
	std::string sArg;
	Block* blkPrev;
	Block* blkNextIfTrue;
	Block* blkNextIfFalse;
	Conditional* cndNextIfTrue;
	Conditional* cndNextIfFalse;
	Vertex* vCondition;
};
#endif