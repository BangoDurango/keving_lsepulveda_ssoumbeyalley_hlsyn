#include "State.h"
int State::sCount;

State::State()
{
	sCount++;
}

State::State(int t, std::string s)
{
	stringstream ss;
	ss << s << t;
	sName = ss.str();
	ss.str("");
	ss.clear();
	time = t;
	sCount++;
}

void State::addVertex(Vertex * v)
{
	nodes.push_back(v);
}

std::vector<Vertex*> State::getNodes()
{
	return nodes;
}

int State::getTime()
{
	return time;
}
std::string tabs(int n) {
	stringstream ss;
	for (int i = 0; i < n; ++i) {
		ss << "\t";
	}
	return ss.str();
}
std::vector<string> State::getVerilog()
{
	std::vector<string> vLines;
	std::string s;

	s = this->sName + ": " + tabs(2) + "\nbegin\n";
	vLines.push_back(s);

	std::string sT, sF;
	string stemp;
	this->getLines();

	for (std::vector<string>::iterator it = slines.begin(); it != slines.end(); ++it) {
		
		if (!(*it).find("if")) {
			vLines.push_back(tabs(3) + *it + "\n");
		}
		else {
			vLines.push_back(tabs(3) + *it + ";\n");
		}
		

		if (sName.at(0) == 'C') {
			if (this->nextIfTrue != NULL) {
				sT = "state = " + this->nextIfTrue->sName + ";\n";
				vLines.push_back(sT);
			}
			if (this->nextIfFalse != NULL) {
				stemp = this->nextIfFalse->getName();
				if (stemp.at(0) == 'E') {
					vLines.push_back("else");
					sF = "\nnextstate = " + this->nextIfFalse->sName + ";\n";
					vLines.push_back(sF);
				}
			
			}
		}
		else {
			if (this->nextIfTrue != NULL) {
				s = "\nnextstate = " + this->nextIfTrue->sName + ";\n";
				vLines.push_back(s);
			}
		}
	}
	
	s = tabs(2) + "end\n\n";
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
		tmpS.push_back((*vIt)->getString() + "\n");
	}
	slines = tmpS;
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
