#include "ControlGraph.h"

void ControlGraph::addBlock(Block * f)
{
	blocks.push_back(f);
}

void ControlGraph::addConditional(Conditional * c)
{
	conditionals.push_back(c);
}
