#pragma once
#ifndef CONTROLGRAPH_H
#define CONTROLGRAPH_H

#include "main.h"
#include "Block.h"
#include "Conditional.h"

class ControlGraph {

public:
	
	void addBlock(Block* f);
	void addConditional(Conditional *c);
	Block* dropLast();
	std::vector<Block*> getBlocks();
	std::vector<Conditional*> getConditionals();
private:
	std::vector<Block*> blocks;
	std::vector<Conditional*> conditionals;
};
#endif