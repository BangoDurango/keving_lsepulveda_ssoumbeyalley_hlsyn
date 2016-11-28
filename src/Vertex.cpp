#include "Vertex.h"
Vertex::Vertex() {
	isCond = false;
}
Vertex::Vertex(int n)
{
	//Vertex();
	isCond = false;
	ID = n;
}

Vertex::Vertex(int n, std::string strType)
{
	//Vertex(n);
	isCond = false;
	ID = n;
	sType = strType;
}

string Vertex::checkValidOp(std::string s)
{
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


	return INVALID;
}

void Vertex::setString(string s)
{
	this->str = s;
}

void Vertex::setID(int n)
{
	ID = n;
}

int Vertex::getID()
{
	return ID;
}

void Vertex::setType(string s)
{
	sType = s;
}

std::string Vertex::getType()
{
	return sType;
}

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
	//	std::string sE0, sE1, sE2;
	//
	//	sE0 = E0->getID();
	//	sE1 = E1->getID();
	//	sE2 = E2->getID();
	//
	//	//std::cout << "Vertex:\t" << this->sType << std::endl;
	//	if (sType != CONDITIONAL) {
	//		std::cout << sE0 << " = " << sE1 << sType << sE2 << std::endl << std::endl;
	//	}
	//}
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

void Vertex::setIfTrue(Edge * e)
{
	ifTrue = e;
}

void Vertex::setElse(Edge * e)
{
	Else e;
}
