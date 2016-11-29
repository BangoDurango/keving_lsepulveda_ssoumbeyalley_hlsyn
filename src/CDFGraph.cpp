#include "CDFGraph.h"

Resource CDFGraph::ADDER_SUBTRACTOR;
Resource CDFGraph::MULTIPLIER;
Resource CDFGraph::LOGICAL;
Resource CDFGraph::IF_STATEMENT;

CDFGraph::CDFGraph()
{
    std::vector<Resource*> rVec;
	INOP.name = "INOP";
	ONOP.name = "ONOP";

	ADDER_SUBTRACTOR.cnt = 0;
	ADDER_SUBTRACTOR.delay = 1;
	ADDER_SUBTRACTOR.name = "Add/Sub";
	ADDER_SUBTRACTOR.ops.push_back("+");
	ADDER_SUBTRACTOR.ops.push_back("-");
	rVec.push_back(&ADDER_SUBTRACTOR);

	MULTIPLIER.cnt = 0;
	MULTIPLIER.delay = 2;
	MULTIPLIER.name = "Mult";
	MULTIPLIER.ops.push_back("*"); 
	rVec.push_back(&MULTIPLIER);

	LOGICAL.name = "Logical";
	LOGICAL.cnt = 0;
	LOGICAL.delay = 1;
	LOGICAL.ops.push_back(GT);
	LOGICAL.ops.push_back(LT);
	LOGICAL.ops.push_back(ET);
	LOGICAL.ops.push_back(SL);
	LOGICAL.ops.push_back(SR);
	rVec.push_back(&LOGICAL);

	IF_STATEMENT.name = "If/Else";
	IF_STATEMENT.ops.push_back(IF);
	IF_STATEMENT.ops.push_back(ELSE);
	IF_STATEMENT.cnt = 0;
	IF_STATEMENT.delay = 1;
	rVec.push_back(&IF_STATEMENT);

	Vertex::resources = rVec;

	vONOP = new Vertex(0, &ONOP);
	vINOP = new Vertex(0, &INOP);
	vONOP->setString("OUTPUTS");
	vINOP->setString("INPUTS");
	//AddSubCnt = 0;
	//MultCnt = 0;
	//LogicCnt = 0;
	//IfCnt = 0;
	//cFlag = false;
	currBlk = new Block;
	currBlk->addVertex(vINOP);
	gControlGraph.addBlock(currBlk);
	StartBlock = currBlk;
	_last = FUNCTION;
}

void CDFGraph::setLatency(int n)
{
	if (n <= 0) {
		std::cout << "Negative or zero latency" << std::endl;
		exit(1);
	}
	latency = n;
}

int CDFGraph::getLatency()
{
	return latency;
}

void CDFGraph::loadFileStrings(std::vector<string> strVec)
{
	this->FileStrings = strVec;
	std::cout << "Raw Strings Loaded." << std::endl;
}

void CDFGraph::loadIOV(std::vector<IOV> ins, std::vector<IOV> outs, std::vector<IOV> vars)
{
	inputs = ins;
	outputs = outs;
	variables = vars;
	std::cout << "IOVs Loaded." << std::endl;
}

IOV* CDFGraph::getIOVbyName(std::string s)
{
	for (std::vector<IOV>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
		if (it->getName() == s) {
			return &(*it);
		}
	}
	for (std::vector<IOV>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
		if (it->getName() == s) {
			return &(*it);
		}
	}
	for (std::vector<IOV>::iterator it = variables.begin(); it != variables.end(); ++it) {
		if (it->getName() == s) {
			return &(*it);
		}
	}
	return NULL;

}

void CDFGraph::parseOperations()
{
	Vertex* tempV;
	Conditional* newC;
	Block* newBlk;
	Block* tempF = NULL;
	Block* lastBlock;

	if (this->FileStrings.size() == 0) {
		std::cout << "No File Strings Loaded to Graph" << std::endl;
		exit(1);
	}
	for (std::vector<string>::iterator it = FileStrings.begin(); it != FileStrings.end(); ++it) {
		
		if (std::size_t found = it->find(IF) != std::string::npos) {
			newC = new Conditional(*it);
			tempV = parseConditional(*it);
			Vertices.push_back(tempV);
			newC->setVCondition(tempV);

			if (_last == FUNCTION) {
				currBlk->setNext(newC);
			}
			else if (_last == CONDITIONAL) {
				currC->setNextIfTrue(newC);
			}
			else {
				std::cout << "Something went wrong (Parsing conditionals)" << std::endl;
			}
			CondVec.push_back(newC);//pushing conditional to the end of this vector because we need to know the order they occured in
			currC = newC;
			gControlGraph.addConditional(currC);

			
			_last = CONDITIONAL;
		}
		else if (std::size_t found = it->find(ELSE) != std::string::npos) {
		
			tempF = currBlk->updatePrev(currC);
		}
		else if (std::size_t found = it->find("}") != std::string::npos) {
			newBlk = new Block();
			currBlk->setNext(newBlk);
			newBlk->setPrev(currBlk);	
			lastBlock = currBlk;
			currBlk = newBlk;
			gControlGraph.addBlock(currBlk);
			if (tempF != NULL) {
				tempF->setNext(newBlk);
				tempF = NULL;
			}
			else {
				CondVec.back()->setNextIfFalse(currBlk);
				CondVec.pop_back();
			}
			
			
			_last = FUNCTION;

		}
		else {
			if (_last == CONDITIONAL) {
				currBlk = new Block();
				currBlk->setPrev(currC);
				currC->setNextIfTrue(currBlk);
				gControlGraph.addBlock(currBlk);
				_last = FUNCTION;
			}
			parseOperation(*it);
		}
	}
	//if (currBlk->getNodes().size() == 0) {
	//	//currBlk = lastBlock;
	//	//gControlGraph.getBlocks().erase(gControlGraph.getBlocks().end());
	currBlk = gControlGraph.dropLast();
	//}
	//currBlk = new Block;
		
	currBlk->addVertex(vONOP);
	//currC->setNextIfFalse(currBlk);
	CDFGraph::addConditionalVertices();
}

void CDFGraph::addConditionalVertices() {
	std::vector<Conditional*> cndVec = gControlGraph.getConditionals();
	if (cndVec.size() == 0) {
		std::cout << "Block vector Empty " << std::endl;
		return;
		//exit(1);
	}
	int cnt = 0;

	Block* currBlk;
	Conditional* currCnd;
	Edge* newE;
	Vertex* currV;
	std::string s;

	for (std::vector<Conditional*>::iterator it = cndVec.begin(); it != cndVec.end(); ++it) {

		(*it)->connectVCnd();
		
		if ((*it)->getVCondition()->getOutgoing().front()->getOutput() == NULL) {
			(*it)->getVCondition()->fixOutGoing();
			//This is ugly and I hate it.
		}
		cnt = 0;
	}
}
std::vector<Edge*> CDFGraph::getEdgesByID(string s)
{
	std::vector<Edge*> vec;
	for (std::vector<Edge*>::iterator it = Edges.begin(); it != Edges.end(); ++it) {
		if ((*it)->getID() == s) {
			vec.push_back(*it);
		}
	}
	return vec;
}

std::vector<Vertex*> CDFGraph::getVertices()
{
	return Vertices;
}

Vertex* CDFGraph::parseConditional(string s) {
	Vertex* newV;
	int nID;
	std::vector<string> tok;
	tok = Parser::splitByWhitespace(s);
	//IfCnt++;
	//IF_STATEMENT.cnt++;
	//nID = IF_STATEMENT.cnt;
	newV = new Vertex();
	newV->setType(&IF_STATEMENT);

	CDFGraph::parseInput(tok.at(2), newV);
	CDFGraph::parseOutput(tok.at(2), newV);
	newV->setString(s);
	return newV;
}
void CDFGraph::parseOperation(string s) {

	std::vector<string> tok;

	Vertex* newV;
	

	tok = Parser::splitByWhitespace(s);
	if (tok.size() == 5) {
		std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;

		//std::string tp = Vertex::checkValidOp(tok.at(3));
		Resource* tp = Vertex::checkValidOp(tok.at(3));
		if (tp != NULL) {
			int nID;
			if (tp == &ADDER_SUBTRACTOR) {
				/*this->AddSubCnt++;
				nID = AddSubCnt;*/
				//ADDER_SUBTRACTOR.cnt++;
				//nID = ADDER_SUBTRACTOR.cnt;
				newV = new Vertex();
				newV->setType(tp);
			}
			else if (tp == &MULTIPLIER) {
				/*this->MultCnt++;
				nID = MultCnt;*/
				//MULTIPLIER.cnt++;
				//nID = MULTIPLIER.cnt;
				newV = new Vertex();
				newV->setType(&MULTIPLIER);
			}
			//else if (tp == LT || tp == GT || tp == ET || tp == SL || tp == SR) {
			else if (tp == &LOGICAL){
				//this->LogicCnt++;
				//nID = LogicCnt;
				//LOGICAL.cnt++;
				//nID = LOGICAL.cnt;
				newV = new Vertex();
				newV->setType(tp);
			}
		}
		
		CDFGraph::parseOutput(tok.at(0), newV);
		CDFGraph::parseInput(tok.at(2), newV);
		CDFGraph::parseInput(tok.at(4), newV);
		
		//if (cFlag == true) {
		//	
		//	newV->addIncoming(conditionals.back());
		//	
		//	cFlag = false;
		//}
		newV->setString(s);
		Vertices.push_back(newV);
		currBlk->addVertex(newV);
	}
}
void CDFGraph::parseInput(string s, Vertex* newV) {

	Edge* newEdge;
	std::vector<Edge*> eVec;
	Edge* eTMP;


	if (getIOVbyName(s)->getType() == INPUT) {
		newEdge = new Edge(eINOP, s);
		//newEdge->setInput(vINOP);
		vINOP->addOutgoing(newEdge);
		newV->addIncoming(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {

		eVec = CDFGraph::getEdgesByID(s);
		if (eVec.size() == 0) {
			newEdge = new Edge(VARIABLE, s);
			//newEdge->setOutput(newV);
			//newEdge->setInput(vINOP);
			this->Edges.push_back(newEdge);
			this->currBlk->addEdge(newEdge);
		}
		else {
			for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
				//Get all the Edges which use this variable
				if ((*it)->getOutput() == NULL) {
					//(*it)->setOutput(newV);//if the edge doesn't have an output, make it this node. 
					newV->addIncoming(*it);
				}
				else {
					eTMP = new Edge(VARIABLE, s);
					//make a new edge leading to this node, and connect it to whichever node
					//eTMP->setOutput(newV);
					(*it)->getInput()->addOutgoing(eTMP);
					newV->addIncoming(eTMP);
					this->Edges.push_back(eTMP);
					this->currBlk->addEdge(eTMP);
				}

			}
		}
	}

}


void CDFGraph::parseOutput(string s, Vertex* newV) {

	Edge* newEdge;
//	Edge* eTMP;
	/***********************output (0)****************************/
	if (getIOVbyName(s)->getType() == OUTPUT) {
		newEdge = new Edge(eONOP, s);
		//newEdge->setInput(newV);
		vONOP->addIncoming(newEdge);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {
		newEdge = new Edge(VARIABLE, s);
		//newEdge->setInput(newV);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}

}
//void CDFGraph::parseIF(string s, Conditional *c) {
//	std::vector<string> tok;
//	Edge* newE;
//	//Vertex* newV;
//
//	tok = Parser::splitByWhitespace(s);
//	if (tok.size() == 5) {
//		std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;
//		
//	}
//
//
//}
void CDFGraph::printGraph() {
	std::cout << "Printing graph:" << std::endl;
	std::cout << "Contents of Vertex Vector: " << std::endl << std::endl;

	Vertex* vcurr;
	Vertex* vnext;
	Edge* eCurr;
	vcurr = this->vONOP;

}

void CDFGraph::DFS(CDFGraph * g, Vertex * v)
{	
	Vertex* nxt;
	std::vector<Edge*> eVec = v->getOutgoing();
	v->visit();
	for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
		nxt = (*it)->getInput();
		if (nxt->checkVisited() == false) {
			DFS(g, nxt);
		}
	}
}

void CDFGraph::ALAP(CDFGraph * g, Vertex * v, int time)
{
	Vertex* nxt;
	//int nxtTime = time - 1;
	if (time < 0) {
		std::cout << "Latency constraint insufficient to schedule all operations." << std::endl;
		exit(1);
	}
	
	std::vector<Edge*> eVec = v->getIncoming();
	//v->visit();

	for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
	
		nxt = (*it)->getInput();
		if (nxt->getType() != &INOP) {
			if (nxt->getALAPTime() > time) {
				nxt->setALAPTime(time);
				ALAP(g, nxt, time - 1);
			}

		}
	}

}
void CDFGraph::ALAP(CDFGraph * g, int n) {
	g->ALAP(n);
}
void CDFGraph::ALAP(int n)
{
	//Vertex::setLatency(n);
	Vertex::latency = n + 1;
	for (std::vector<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
		(*it)->setALAPTime(Vertex::latency);
	}
	ALAP(this, vONOP, n); 
}
void CDFGraph::LIST_R(CDFGraph * g, Vertex * v) {
	//g->LIST_R()
}

void CDFGraph::LIST_R(int n) {
	



	CDFGraph::ALAP(this, n);
	//LIST_R(this, )
	int t = 1;

}
