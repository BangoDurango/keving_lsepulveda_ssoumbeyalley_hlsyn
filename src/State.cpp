#include "State.h"
int State::sCount;

State::State()
{
	sCount++;
}

State::State(int t, std::string s)
{
	sName = s + std::to_string(t);
	time = t;
	sCount++;
}

void State::addVertex(Vertex * v)
{
	nodes.push_back(v);
}
State* State::combineIfElseStates(State * s1, State * s2, Vertex * vCond)
{/*
	
	std::vector<Vertex*> n1;
	std::vector<Vertex*> n2;
	int time = s1->getTime();
	State* newS = new State(time );
	if (time == s2->getTime ) {
		n1 = s1->getNodes();
		n1.insert()
	}*/
	return nullptr;
}
std::vector<Vertex*> State::getNodes()
{
	return nodes;
}

int State::getTime()
{
	return time;
}

std::vector<string> State::getVerilog()
{
	std::vector<string> vLines;
	std::string s;

	s = this->sName + ": " + "begin\n";
	vLines.push_back(s);

	std::string sT, sF;

	for (std::vector<string>::iterator it = slines.begin(); it != slines.end(); ++it) {
		
		vLines.push_back(*it);

		if (sName.substr(0) == "C") {
			
			sT = "state = " + this->nextIfTrue->sName + ";\n";

		}
	}

	s = "end\n";
	vLines.push_back(s);

	return vLines;
}

//void State::setNext(State * n)
//{
//	nextState = n;
//}

std::vector<string> State::getStrings()
{
	std::vector<string> tmpS;

	for (std::vector<Vertex*>::iterator vIt = nodes.begin(); vIt != nodes.end(); ++vIt) {
		tmpS.push_back((*vIt)->getString());
	}

	return tmpS;
}

void State::setName(std::string s){
	sName = s;
}
std::string State::getName(){
	return sName;
}
void State::setNextIfTrue(State* s){
	nextIfTrue = s;
}
void State::setNextIfFalse(State* s){
	nextIfFalse = s;
}

void State::setNextIfTrue(std::vector<State*> s){
	nextIfTrue = s.front();
}
void State::setNextIfFalse(std::vector<State*> s){
	nextIfFalse = s.front();
}

State* State::combineStates(State* s1, State* conditional){
	State* newS = NULL;
	if (s1->getTime() == conditional->getTime()){
		newS = new State(conditional->getTime(), "S_");
		std::vector<Vertex*> vVec = s1->getNodes();
		
		for (std::vector<Vertex*>::iterator vIt = vVec.begin(); vIt != vVec.end(); ++vIt) {
			newS->addVertex(*vIt);
			//newS->addLine((*vIt)->getString());
		}
		newS->addVertex(conditional->getNodes().front());
		newS->addLine(conditional->getNodes().front()->getString());
		//newS->setNextIfTrue(conditional->getNextIfTrue());
		//newS->setNextIfFalse(conditional->getNextIfFalse());
	}
	return newS;
}
State* State::getNextIfTrue(){
	return nextIfTrue;
}
State* State::getNextIfFalse(){
	return nextIfFalse;
}
std::vector<std::string> State::getLines(){
	string s;
	for (std::vector<Vertex*>::iterator vIt = nodes.begin(); vIt != nodes.end(); ++vIt) {
		s = (*vIt)->getString();
		if (s != "INPUTS" && s != "OUTPUTS") {
			slines.push_back(s);
		}
	}
	return slines;
}
void State::addLine(std::string s){
	slines.push_back(s);
}

void State::printLines() {
	this->getLines();

	for (std::vector<string>::iterator it = slines.begin(); it != slines.end(); ++it) {
		std::cout << *it << std::endl;
	}

}

void State::resetStateCount()
{
	sCount = 0;
}

int State::getStateCount()
{
	return sCount;
}
