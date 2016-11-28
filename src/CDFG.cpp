#include "CDFG.h"

void CDFG::addFunction(Func * f)
{
	functions.push_back(f);
}

void CDFG::addConditional(Conditional * c)
{
	conditionals.push_back(c);
}
