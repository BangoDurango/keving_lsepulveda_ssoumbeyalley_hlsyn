#include "CDFGraph.h"

CDFGraph::CDFGraph()
{
	vONOP = new Vertex(0, ONOP);
	vINOP = new Vertex(0, INOP);
	vONOP->setString("OUTPUTS");
	vINOP->setString("INPUTS");
	AddSubCnt = 0;
	MultCnt = 0;
	LogicCnt = 0;
	IfCnt = 0;
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

	Conditional* newC;
	Block* newF;
	Block* tempF;

	if (this->FileStrings.size() == 0) {
		std::cout << "No File Strings Loaded to Graph" << std::endl;
		exit(1);
	}
	for (std::vector<string>::iterator it = FileStrings.begin(); it != FileStrings.end(); ++it) {
		
		if (std::size_t found = it->find(IF) != std::string::npos) {
			newC = new Conditional(*it);
			
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
			_last = CONDITIONAL;
		}
		else if (std::size_t found = it->find(ELSE) != std::string::npos) {
		
			tempF = currBlk->updatePrev(currC);
		}
		else if (std::size_t found = it->find("}") != std::string::npos) {
			newF = new Block();
			currBlk->setNext(newF);
			newF->setPrev(currBlk);
			currBlk = newF;
			if (tempF != NULL) {
				tempF->setNext(newF);
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
				_last = FUNCTION;
			}
			parseOperation(*it);
		}
	}
	currBlk->addVertex(vONOP);
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


void CDFGraph::parseOperation(string s) {

	std::vector<string> tok;

	Vertex* newV;

	tok = Parser::splitByWhitespace(s);
	if (tok.size() == 5) {
		std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;

		std::string tp = Vertex::checkValidOp(tok.at(3));
		if (tp != INVALID) {
			int nID;
			if (tp == PLUS || tp == MINUS) {
				this->AddSubCnt++;
				nID = AddSubCnt;
				newV = new Vertex(nID);
				newV->setType(tp);
			}
			else if (tp == MULT) {
				this->MultCnt++;
				nID = MultCnt;
				newV = new Vertex(nID);
				newV->setType(MULT);
			}
			else if (tp == LT || tp == GT || tp == ET || tp == SL || tp == SR) {
				this->LogicCnt++;
				nID = LogicCnt;
				newV = new Vertex(nID);
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
		Vertices.push_back(newV);
		currBlk->addVertex(newV);
	}
}
void CDFGraph::parseInput(string s, Vertex* newV) {

	Edge* newEdge;
	std::vector<Edge*> eVec;
	Edge* eTMP;


	if (getIOVbyName(s)->getType() == INPUT) {
		newEdge = new Edge(INOP, s);
		newEdge->setInput(vINOP);
		vINOP->addOutgoing(newEdge);
		newV->addIncoming(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {

		eVec = CDFGraph::getEdgesByID(s);
		if (eVec.size() == 0) {
			newEdge = new Edge(VARIABLE, s);
			newEdge->setOutput(newV);
			newEdge->setInput(vINOP);
			this->Edges.push_back(newEdge);
			this->currBlk->addEdge(newEdge);
		}
		else {
			for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
				//Get all the Edges which use this variable
				if ((*it)->getOutput() == NULL) {
					(*it)->setOutput(newV);//if the edge doesn't have an output, make it this node. 
				}
				else {
					eTMP = new Edge(VARIABLE, s);
					//make a new edge leading to this node, and connect it to whichever node
					eTMP->setOutput(newV);
					(*it)->getInput()->addOutgoing(eTMP);
					this->Edges.push_back(eTMP);
					this->currBlk->addEdge(eTMP);
				}

			}
		}
	}
	else {

	}
}


void CDFGraph::parseOutput(string s, Vertex* newV) {

	Edge* newEdge;
//	Edge* eTMP;
	/***********************output (0)****************************/
	if (getIOVbyName(s)->getType() == OUTPUT) {
		newEdge = new Edge(ONOP, s);
		vONOP->addIncoming(newEdge);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {
		newEdge = new Edge(VARIABLE, s);
		newEdge->setInput(newV);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currBlk->addEdge(newEdge);
	}
	else {

	}
}
void CDFGraph::parseIF(string s, Conditional *c) {
	std::vector<string> tok;
	Edge* newE;
	//Vertex* newV;

	tok = Parser::splitByWhitespace(s);
	if (tok.size() == 5) {
		std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;
		
	}


}
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
	
	if (time < 1) {
		std::cout << "Latency constraint insufficient to schedule all operations." << std::endl;
		exit(1);
	}
	
	std::vector<Edge*> eVec = v->getIncoming();
	//v->visit();

	for (std::vector<Edge*>::iterator it = eVec.begin(); it != eVec.end(); ++it) {
	
		nxt = (*it)->getInput();
		if (nxt->getType() != INOP) {
			if (nxt->getALAPTime() > time) {
				nxt->setALAPTime(time);
				ALAP(g, nxt, time - 1);
			}

		}
	}

}

void CDFGraph::ALAP(int n)
{
	//Vertex::setLatency(n);
	Vertex::latency = n;
	ALAP(this, vONOP, n); 
}
