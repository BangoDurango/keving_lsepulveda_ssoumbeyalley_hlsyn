#include "Graph.h"

Graph::Graph()
{
	vNOP = new Vertex(0, NOP);
	vINOP = new Vertex(0, INOP);
	vNOP->setString("OUTPUTS");
	vINOP->setString("INPUTS");
	AddSubCnt = 0;
	MultCnt = 0;
	LogicCnt = 0;
	IfCnt = 0;
	//cFlag = false;
	currF = new Func;
	currF->addVertex(vINOP);
	gCDFG.addFunction(currF);
	_last = FUNCTION;
}

void Graph::loadFileStrings(std::vector<string> strVec)
{
	this->FileStrings = strVec;
	std::cout << "Raw Strings Loaded." << std::endl;
}

void Graph::loadIOV(std::vector<IOV> ins, std::vector<IOV> outs, std::vector<IOV> vars)
{
	inputs = ins;
	outputs = outs;
	variables = vars;
	std::cout << "IOVs Loaded." << std::endl;
}

IOV* Graph::getIOVbyName(std::string s)
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

void Graph::parseOperations()
{
	//conditionalFlag = false;
	Conditional* newC;
	Func* newF;
	Func* tempF;

	if (this->FileStrings.size() == 0) {
		std::cout << "No File Strings Loaded to Graph" << std::endl;
		exit(0);
	}
	for (std::vector<string>::iterator it = FileStrings.begin(); it != FileStrings.end(); ++it) {
		
		if (std::size_t found = it->find(IF) != std::string::npos) {
			newC = new Conditional(*it);
			
			if (_last == FUNCTION) {
				currF->setNext(newC);
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
		
			tempF = currF->updatePrev(currC);
		}
		else if (std::size_t found = it->find("}") != std::string::npos) {
			newF = new Func();
			currF->setNext(newF);
			newF->setPrev(currF);
			currF = newF;
			if (tempF != NULL) {
				tempF->setNext(newF);
				tempF = NULL;
			}
			else {
				CondVec.back()->setNextIfFalse(currF);
				CondVec.pop_back();
			}
			
			
			_last = FUNCTION;

		}
		else {
			if (_last == CONDITIONAL) {
				currF = new Func();
				currF->setPrev(currC);
				currC->setNextIfTrue(currF);
				_last = FUNCTION;
			}
			parseOperation(*it);
		}
	}
	currF->addVertex(vNOP);
}

std::vector<Edge*> Graph::getEdgesByID(string s)
{
	std::vector<Edge*> vec;
	for (std::vector<Edge*>::iterator it = Edges.begin(); it != Edges.end(); ++it) {
		if ((*it)->getID() == s) {
			vec.push_back(*it);
		}
	}
	return vec;
}

void Graph::parseOperation(string s) {

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
		
		Graph::parseOutput(tok.at(0), newV);
		Graph::parseInput(tok.at(2), newV);
		Graph::parseInput(tok.at(4), newV);
		
		//if (cFlag == true) {
		//	
		//	newV->addIncoming(conditionals.back());
		//	
		//	cFlag = false;
		//}
		Vertices.push_back(newV);
		currF->addVertex(newV);
	}
}
void Graph::parseInput(string s, Vertex* newV) {

	Edge* newEdge;
	std::vector<Edge*> eVec;
	Edge* eTMP;


	if (getIOVbyName(s)->getType() == INPUT) {
		newEdge = new Edge(INOP, s);
		newEdge->setInput(vINOP);
		vINOP->addOutgoing(newEdge);
		newV->addIncoming(newEdge);
		this->Edges.push_back(newEdge);
		this->currF->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {

		eVec = Graph::getEdgesByID(s);
		if (eVec.size() == 0) {
			newEdge = new Edge(VARIABLE, s);
			newEdge->setOutput(newV);
			newEdge->setInput(vINOP);
			this->Edges.push_back(newEdge);
			this->currF->addEdge(newEdge);
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
					this->currF->addEdge(eTMP);
				}

			}
		}
	}
	else {

	}
}


void Graph::parseOutput(string s, Vertex* newV) {

	Edge* newEdge;
//	Edge* eTMP;
	/***********************output (0)****************************/
	if (getIOVbyName(s)->getType() == OUTPUT) {
		newEdge = new Edge(NOP, s);
		vNOP->addIncoming(newEdge);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currF->addEdge(newEdge);
	}
	else if (getIOVbyName(s)->getType() == VARIABLE) {
		newEdge = new Edge(VARIABLE, s);
		newEdge->setInput(newV);
		newV->addOutgoing(newEdge);
		this->Edges.push_back(newEdge);
		this->currF->addEdge(newEdge);
	}
	else {

	}
}
void Graph::parseIF(string s, Conditional *c) {
	std::vector<string> tok;
	Edge* newE;
	//Vertex* newV;

	tok = Parser::splitByWhitespace(s);
	if (tok.size() == 5) {
		std::cout << tok.at(0) << '\t' << tok.at(1) << '\t' << tok.at(2) << '\t' << tok.at(3) << '\t' << tok.at(4) << std::endl;
		
		
	}


}
void Graph::printGraph() {
	std::cout << "Printing graph:" << std::endl;
	std::cout << "Contents of Vertex Vector: " << std::endl << std::endl;

	Vertex* vcurr;
	Vertex* vnext;
	Edge* eCurr;
	vcurr = this->vNOP;

	//for (std::vector<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
	//	(*it)->printVertex();
	//}
	//std::cout << "Using IOV inputs: " << std::endl << std::endl;
	//for (std::vector<IOV>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
	//	eCurr = getEdgeByID(it->getName());
	//	std::cout << "Starting input: " << it->getName() << std::endl;
	//	vcurr = eCurr->getOutput();
	//	do {
	//		//vcurr = eCurr->getOutput();
	//		if (vcurr == NULL) {
	//			std::cout << "Null pointer, something happened iterating through graph (printGraph())" << std::endl;
	//			break;
	//		}
	//		vcurr->printVertex();
	//		eCurr = vcurr->getE0();
	//		vcurr = eCurr->getOutput();
	//	} while (vcurr->getType() != NOP);
	//}
}