#pragma once
#ifndef IF_H
#define IF_H

#include "main.h"

class Func;
class Conditional {

public:
	Conditional();
	Conditional(std::string Arg);
	Conditional(std::string s, Func* fPrev);
	void setNextIfTrue(Func* f);
	void setNextIfTrue(Conditional *c);
	void setNextIfFalse(Func* f);
	void setNextIfFalse(Conditional *c);



private:
	std::string sArg;
	Func* fPrev;
	Func* fNextIfTrue;
	Func* fNextIfFalse;
	Conditional* cNextIfTrue;
	Conditional* cNextIfFalse;

};
#endif