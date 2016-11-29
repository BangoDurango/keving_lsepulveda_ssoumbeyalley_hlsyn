#include "Vertex.h"
int Vertex::latency;
std::vector<Resource*> Vertex::resources;


Vertex::Vertex() {
	rType = NULL;
	visited = false;
	strNode = "a";
	ALAPtime = latency;
	//ALAPtime = CDFGraph::latency;
}
//Vertex::Vertex(int n)
//{
//	rType = NULL;
//	strNode = "b";
//	ALAPtime = Vertex::latency;
//	visited = false;
//	ID = n;
//}
//
//Vertex::Vertex(int n, std::string strType)
//{
//	strNode = "c";
//	ALAPtime = Vertex::latency;
//	visited = false;
//	ID = n;
//	sType = strType;
//}

Vertex::Vertex(int n, Resource* inRType)
{
		ALAPtime = Vertex::latency;
		visited = false;
		ID = n;
		//sType = strType;
		rType = inRType;
}

Resource* Vertex::checkValidOp(std::string s)
{
	//std::vector<Resource*> rVec;
	bool flag = false;
	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it){
		//if ((*it)->)
		for (std::vector<std::string>::iterator sIt = (*it)->ops.begin(); sIt != (*it)->ops.end(); ++it){
			return *it;
		}
	}
	return NULL;
/*
	if (s == "+") {
		return PLUS;
	}
	else if (s == "-") {
		return MINUS;
	}
	else if (s == "*") {
		return MULT;
	}
	else if (s == ">") {
		return GT;
	}
	else if (s == "<") {
		return LT;
	}
	else if (s == "==") {
		return ET;
	}
	else if (s == "?") {
		return MUX;
	}
	else if (s == ">>") {
		return SR;
	}
	else if (s == "<<") {
		return SL;
	}


	return INVALID;*/
}

void Vertex::setString(std::string s)
{
	this->strNode = s;
}

std::string Vertex::getString()
{
	return strNode;
}

void Vertex::setID(int n)
{
	ID = n;
}

int Vertex::getID()
{
	return ID;
}

void Vertex::setType(Resource * r)
{	
	int nCnt = r->cnt + 1;
	r->cnt = nCnt;
	this->ID = r->cnt;
	rType = r;
}

Resource * Vertex::getType()
{
	return rType;
}

//void Vertex::setType(string s)
//{
//	sType = s;
//}
//
//std::string Vertex::getType()
//{
//	return sType;
//}

//void Vertex::setE1E2(Edge * e1, Edge * e2)
//{
//	E1 = e1;
//	E2 = e2;
//}
//
//void Vertex::setE0(Edge * e0)
//{
//	E0 = e0;
//}
//
//Edge * Vertex::getE0()
//{
//	return E0;
//}

void Vertex::printVertex()
{
		std::string sE0, sE1, sE2, sType;
	
		for (std::vector<Edge*>::iterator it = incoming.begin(); it != incoming.end(); ++it) {
			if((*it)->sType != IF){

			}
		}
}

std::vector<Edge*> Vertex::getOutgoing()
{
	return outgoing;
}

std::vector<Edge*> Vertex::getIncoming()
{
	return incoming;
}

void Vertex::addIncoming(Edge * e)
{
	e->setOutput(this);
	incoming.push_back(e);
}

void Vertex::addOutgoing(Edge * e)
{	
	e->setInput(this);

	outgoing.push_back(e);

}
void Vertex::fixOutGoing() {
	if (outgoing.front()->getOutput() == NULL) {
		outgoing.erase(outgoing.begin());
	}
}
//void Vertex::setVNumber(int n)
//{
//	vNumber = n;
//}
//
//int Vertex::getVNumber()
//{
//	return vNumber;
//}

void Vertex::visit()
{
	this->visited = true;
}

bool Vertex::checkVisited()
{
	return visited;
}

//void Vertex::setLatency(int n)
//{
//	latency = n;
//}

int Vertex::getALAPTime()
{
	return ALAPtime;
}

void Vertex::setALAPTime(int n)
{
	ALAPtime = n;
}
