#include "ControlGraph.h"

void ControlGraph::addBlock(Block * f)
{
	blocks.push_back(f);
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
