#pragma once
#ifndef IF_H
#define IF_H

#include "main.h"

class Block;
class Conditional {

public:
	Conditional();
	Conditional(std::string Arg);
	Conditional(std::string s, Block* fPrev);
	void setNextIfTrue(Block* f);
	void setNextIfTrue(Conditional *c);
	void setNextIfFalse(Block* f);
	void setNextIfFalse(Conditional *c);



private:
	std::string sArg;
	Block* fPrev;
	Block* fNextIfTrue;
	Block* fNextIfFalse;
	Conditional* cNextIfTrue;
	Conditional* cNextIfFalse;

};
#endif