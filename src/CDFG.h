#pragma once
#ifndef CDFG_H
#define CDFG_H

#include "main.h"
#include "Func.h"
#include "Conditional.h"

class CDFG {

public:
	
	void addFunction(Func* f);
	void addConditional(Conditional *c);

private:
	std::vector<Func*> functions;
	std::vector<Conditional*> conditionals;
};
#endif