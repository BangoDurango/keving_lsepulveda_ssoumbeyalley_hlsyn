#include "CDFGraph.h"

Resource CDFGraph::ADDER_SUBTRACTOR;
Resource CDFGraph::MULTIPLIER;
Resource CDFGraph::LOGICAL;
Resource CDFGraph::IF_STATEMENT;

CDFGraph::CDFGraph()
{
  
	INOP.name = "INOP";
	ONOP.name = "ONOP";

	ADDER_SUBTRACTOR.cnt = 0;
	ADDER_SUBTRACTOR.delay = 1;
	ADDER_SUBTRACTOR.name = "Add/Sub";
	ADDER_SUBTRACTOR.inUse = 0;
	ADDER_SUBTRACTOR.ops.push_back("+");
	ADDER_SUBTRACTOR.ops.push_back("-");
	rVec.push_back(&ADDER_SUBTRACTOR);

	MULTIPLIER.cnt = 0;
	MULTIPLIER.inUse = 0;
	MULTIPLIER.delay = 2;
	MULTIPLIER.name = "Mult";
	MULTIPLIER.ops.push_back("*"); 
	rVec.push_back(&MULTIPLIER);

	LOGICAL.name = "Logical";
	LOGICAL.cnt = 0;
	LOGICAL.inUse = 0;
	LOGICAL.delay = 1;
	LOGICAL.ops.push_back(GT);
	LOGICAL.ops.push_back(LT);
	LOGICAL.ops.push_back(ET);
	LOGICAL.ops.push_back(SL);
	LOGICAL.ops.push_back(SR);
	LOGICAL.ops.push_back(MUX);
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
	//std::cout << "Raw Strings Loaded." << std::endl;
}

void CDFGraph::loadIOV(std::vector<IOV> ins, std::vector<IOV> outs, std::vector<IOV> vars)
{
	inputs = ins;
	outputs = outs;
	variables = vars;
	//std::cout << "IOVs Loaded." << std::endl;
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
		
			tempF = currBlk->convertToElse(currC);
			//currBlk->setToElse();
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
				currBlk->setToIf();
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
		
	
	//currC->setNextIfFalse(currBlk);
	CDFGraph::addConditionalVertices();
	for (std::vector<Edge*>::iterator it = Edges.begin(); it != Edges.end(); ++it) {
		if ((*it)->getOutput() == NULL){
			vONOP->addIncoming(*it);
		}
	}
	currBlk->addVertex(vONOP);
}

void CDFGraph::addConditionalVertices() {
	std::vector<Conditional*> cndVec = gControlGraph.getConditionals();
	if (cndVec.size() == 0) {
		//std::cout << "Block vector Empty " << std::endl;
		return;
		//exit(1);
	}
	int cnt = 0;

//	Block* currBlk;
//	Conditional* currCnd;
//	Edge* newE;
//	Vertex* currV;
	std::string s;
	std::vector<Edge*> eVec;
	std::vector<Block*> bVec;
	std::vector<Vertex*> vVec;
	bVec = gControlGraph.getBlocks();

	//for (std::vector<Block*>::iterator bIt = bVec.begin(); bIt != bVec.end(); ++bIt) {

	//	//vVec = (*bIt)->getNodes();

	//	//for (std::vector<Vertex*>::iterator vIt = vVec.begin(); vIt != vVec.end(); ++vIt) {

	//		(*bIt)->checkForVertexInConverse();

	////	}

	//}

	for (std::vector<Conditional*>::iterator it = cndVec.begin(); it != cndVec.end(); ++it) {

		eVec = (*it)->connectVCnd();
		//for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
		//	if ((*it)->getOutput()->getOutgoing().front()->getOutput() == NULL) {
		//		//cnt = 0;
		//		vONOP->addIncoming((*it)->getOutput()->getOutgoing().front());
		//	}
		//}
		Edges.insert(Edges.end(), eVec.begin(), eVec.end());
		
		//if ((*it)->getVCondition()->getOutgoing().front()->getOutput() == NULL) {
		//	(*it)->getVCondition()->fixOutGoing();
		//	//This is ugly and I hate it.
		//}
		cnt = 0;
	}
	if (cndVec.back()->getVCondition()->getOutgoing().front() == NULL) {
		cnt = 0;
	}
}
//std::vector<Vertex*> CDFGraph::getNodesByOutgoingEdgeID(string s)
std::vector<Vertex*> CDFGraph::getVerticesByEdgeID(string s)
{
	//std::cout << s << std::endl;


	//gonna brute force this one because this is taking forever.
	std::vector<Vertex*> vVec;
	bool match = false;
	Vertex* vTMP;
	//std::vector<Edge*> eVec1;
	//std::vector<Edge*> eVec2;

	for (std::vector<Edge*>::iterator eIt1 = Edges.begin(); eIt1 != Edges.end(); ++eIt1) {
		
		if ((*eIt1)->getID() == s) {//get all the edges with the same name
			vTMP = (*eIt1)->getInput();
			match = false;
			if (!vVec.empty() && vTMP != NULL) {
				if (std::find(vVec.begin(), vVec.end(), vTMP) != vVec.end()) {
					match = true;
				}
				//for (std::vector<Vertex*>::iterator vIt = vVec.begin(); vIt != vVec.end(); ++vIt) {//Loop through current list of vertices which output those edges

				//	if ((*vIt)->getString() == vTMP->getString() && (*vIt)->getID() <= vTMP->getID()) {//if this vertex string doesn't match any in the vector
				//		match = true;
				//	}
				//	
				//}
			}
		/*	if (currBlk->getConverse() != NULL) {
				Block* cvBlk = currBlk->getConverse();
				if (!cvBlk->checkForVertex(vTMP)) {
					vVec.push_back(vTMP);
				}
			}*/
			if (match == false) {
				if (currBlk->getConverse() != NULL) {
					Block* cvBlk = currBlk->getConverse();
					if (!cvBlk->checkForVertex(vTMP)) {
						vVec.push_back(vTMP);
					}
				}
				else {
					vVec.push_back(vTMP);
				}
			}
		}
	}
	

	return vVec;
}

std::vector<Vertex*> CDFGraph::getVertices()
{
	return Vertices;
}

Vertex* CDFGraph::parseConditional(string s) {
	Vertex* newV;
//	int nID;
	std::vector<string> tok;
	tok = Parser::splitByWhitespace(s);
	//IfCnt++;
	//IF_STATEMENT.cnt++;
	//nID = IF_STATEMENT.cnt;
	newV = new Vertex();
	newV->setType(&IF_STATEMENT);
	newV->setString(s);

	CDFGraph::parseInput(tok.at(2), newV);
	CDFGraph::parseOutput(tok.at(2), newV);
	
	return newV;
}
void CDFGraph::parseOperation(string s) {

	std::vector<string> tok;

	Vertex* newV;
	//std::cout << s << std::endl;
	tok = Parser::splitByWhitespace(s);
	if (tok.size() == 5) {
	//	std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;

		Resource* tp = Vertex::checkValidOp(tok.at(3));
		if (tp != NULL) {

			newV = new Vertex();
			newV->setType(tp);
			newV->setString(s);

		}

		CDFGraph::parseOutput(tok.at(0), newV);
		CDFGraph::parseInput(tok.at(2), newV);
		CDFGraph::parseInput(tok.at(4), newV);
		
	}
	else if (tok.size() == 7) {
		if (tok.at(3) == MUX) {
			newV = new Vertex();
			newV->setType(&LOGICAL);
			newV->setString(s);
			CDFGraph::parseOutput(tok.at(0), newV);
			CDFGraph::parseInput(tok.at(2), newV);
			CDFGraph::parseInput(tok.at(4), newV);
			CDFGraph::parseInput(tok.at(6), newV);
		}
	}
	
	Vertices.push_back(newV);
	currBlk->addVertex(newV);
}
void CDFGraph::parseInput(string s, Vertex* newV) {

	Edge* newEdge;
	std::vector<Vertex*> vVec;
//	Edge* eTMP;
	if (!checkInputorVariable(s)) {
		std::cout << "Invalid input to operation: " << s << " Not declared: " << newV->getString() << std::endl;
		exit(1);
	}

	if (getIOVbyName(s)->getType() == INPUT) {
		newEdge = new Edge(eINOP, s);

		vINOP->addOutgoing(newEdge);
		newV->addIncoming(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {

		vVec = CDFGraph::getVerticesByEdgeID(s);
		if (vVec.size() == 0) {
			newEdge = new Edge(VARIABLE, s);
			this->Edges.push_back(newEdge);
			this->currBlk->addEdge(newEdge);
		}
		else {
			for (std::vector<Vertex*>::iterator it = vVec.begin(); it != vVec.end(); ++it) {
				

					if ((*it)->getOutgoing().front()->getOutput() != NULL) {
						newEdge = new Edge(VARIABLE, s);
						(*it)->addOutgoing(newEdge);
					}
					else {
						newEdge = (*it)->getOutgoing().front();
					}

					newV->addIncoming(newEdge);
					this->Edges.push_back(newEdge);
					this->currBlk->addEdge(newEdge);
				
			}
		}
	}

}


void CDFGraph::parseOutput(string s, Vertex* newV) {

	Edge* newEdge;
//	Edge* eTMP;
	/***********************output (0)****************************/
	if (!checkOutputorVariable(s)) {
		std::cout << "Invalid output from operation: " << s << " Not declared: " << newV->getString() << std::endl;
		exit(1);
	}
	if (getIOVbyName(s)->getType() == OUTPUT) {
		newEdge = new Edge(eONOP, s);
		//newEdge->setInput(newV);
		vONOP->addIncoming(newEdge);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	if (getIOVbyName(s)->getType() == INPUT) {
		if (newV->getType() == &IF_STATEMENT) {
			newEdge = new Edge(eINOP, s);
			//vINOP->addOutgoing(newEdge);
			newV->addOutgoing(newEdge);
			this->Edges.push_back(newEdge);
			this->currBlk->addEdge(newEdge);
		}
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
	//std::cout << "Printing graph:" << std::endl;
//	std::cout << "Contents of Vertex Vector: " << std::endl << std::endl;

	//Vertex* vcurr;
	//Vertex* vnext;
	//Edge* eCurr;
	//vcurr = this->vONOP;

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
	if (time < -1) {
		std::cout << "Latency constraint insufficient to schedule all operations." << std::endl;
		exit(1);
	}
	
	std::vector<Edge*> eVec = v->getIncoming();
	//v->visit();

	for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
		
		nxt = (*it)->getInput();
		//if(nxt != NULL){
			if (nxt != NULL) {
				/*if (nxt->checkVisited() == false) {
					nxt->setALAPTime(latency);
					nxt->visit();
				}*/
				if (nxt->getALAPTime() > time) {
					nxt->setALAPTime(time);
					ALAP(g, nxt, time - 1);
				}
			//}
		}
	}

}
void CDFGraph::ALAP(CDFGraph * g, int n) {
	g->ALAP(n);
}
void CDFGraph::ALAP(int n)
{
	//Vertex::setLatency(n);
	//Vertex::latency = n + 1;
	//for (std::vector<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
	//	(*it)->setALAPTime(Vertex::latency);
	//}
	ALAP(this, vONOP, n); 
}
void CDFGraph::LIST_R(CDFGraph * g, Vertex * v) {
	//g->LIST_R()
}
bool sortOperator(Vertex *lhs, Vertex *rhs)
{
	bool val;
	val = false;

	if (lhs->getALAPTime() < rhs->getALAPTime()) {
		val = true;
	}

		return val;

}
void CDFGraph::LIST_R(int n) {
	latency = n;

	vONOP->setALAPTime(n + 1);
	ALAP(n);
	
	//CDFGraph::ALAP(this, n);
	//LIST_R(this, )
	int t = 1;
//	Vertex* minALAP;
	int min = 999;
	std::vector<Vertex*> allV = Vertices;//vector full of all of the nodes in the graph, which at this point are as yet unscheduled.
	std::vector<Vertex*> vSchedule;//vector of nodes which have been scheduled?
//	bool found = false;
	bool schFlag = false;



	do { // while (allV.size() > 0);
		std::sort(allV.begin(), allV.end(), sortOperator);//sorts by ALAP time small->large
		bool schFlag = false;
		CDFGraph::resetRCounts();
		//Each loop here is for a different time slot, so start by setting the count of each resource to zero

		for (std::vector<Resource*>::iterator rIt = rVec.begin(); rIt != rVec.end(); ++rIt) {
			//for each type of resource 

			for (std::vector<Vertex*>::iterator vIt1 = allV.begin(); vIt1 != allV.end();) {
				//loop through all the nodes we have
				if ((*vIt1)->getType() == *rIt) {
					//if this node is the same type as the one we're looking at
					if ((*vIt1)->getALAPTime() - t == 0) {
						//if its slack time is zero
						(*vIt1)->scheduleNode(t);
						(*rIt)->cnt++;
						vIt1 = allV.erase(vIt1);
						schFlag = true;
					}
					else if ((*rIt)->cnt < (*rIt)->inUse) {
						//if its slack time is greater than zero but we have resources still available
						//if we still have resources available
						(*vIt1)->scheduleNode(t);
						(*rIt)->cnt++;
						vIt1 = allV.erase(vIt1);
						schFlag = true;
					}
					else if ((*rIt)->cnt > (*rIt)->inUse) {
						(*rIt)->inUse = (*rIt)->cnt;
					}
				}
				if (schFlag == false) {
					++vIt1;
				}
				schFlag = false;
			}
		}
		t++;
	} while (allV.size() > 0);
	for (std::vector<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
		std::cout << left << "Node: [" << (*it)->getString() << "]\t\t" << right << "ALAP Time:" << (*it)->getALAPTime() << "\tScheduled Time: " << (*it)->query_Schedule() << std::endl;
	}
}


void CDFGraph::resetRCounts() {
	for (std::vector<Resource*>::iterator rIt = rVec.begin(); rIt != rVec.end(); ++rIt) {
		(*rIt)->cnt = 0;
	}
}
bool CDFGraph::checkInputorVariable(std::string s) {
	IOV* tIOV = getIOVbyName(s);
	for (std::vector<IOV>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
		if (it->getName() == s) {
			return true;
		}
	}
	for (std::vector<IOV>::iterator it = variables.begin(); it != variables.end(); ++it) {
		if (it->getName() == s) {
			return true;
		}
	}
	return false;

}

bool CDFGraph::checkOutputorVariable(std::string s) {
	IOV* tIOV = getIOVbyName(s);
	for (std::vector<IOV>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
		if (it->getName() == s) {
			return true;
		}
	}
	for (std::vector<IOV>::iterator it = variables.begin(); it != variables.end(); ++it) {
		if (it->getName() == s) {
			return true;
		}
	}
	return false;

}