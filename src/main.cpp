#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include "Parser.h"
#include "InputsOutputs.h"

using namespace std;

int main (int argc, char* argv[]){
	string path = "E:\\Kevin\\Documents\\GitHub\\keving_lsepulveda_ssoumbeyalley_hlsyn\\testfiles\\if tests\\";
	string inFileString = "test_if1.c";
	char* inCStr = new char[(path + inFileString).length() - 1];
	strcpy(inCStr, (path + inFileString).c_str());

	if (argc != 4) {
		std::cout << "Usage: hlsyn cFile latency verilogFile" << std::endl;
		//exit(1);
		//return 0;
	}

	//std::cout << argv[0] << std::endl;

	std::vector<string> rawFileStrings;
	std::vector<InputsOutputs> inputs;
	std::vector<InputsOutputs> outputs;
	std::vector<InputsOutputs> variables;
	//Parser::parseFile(argv[1], rawFileStrings);
	Parser::parseFile(inCStr, &rawFileStrings);

	for (std::vector<string>::iterator it = rawFileStrings.begin(); it != rawFileStrings.end(); ++it) {
		std::cout << *it << std::endl;
	}
	std::cout << "......................................" << std::endl;


	InputsOutputs::generateInputsOutputs(&rawFileStrings, &inputs, &outputs, &variables);
	//Note: This function removes the declaration lines, so only procedural statements remain in rawFileStrings after this. 

	//string dName = Parser::getModuleName(argv[1]);
	std::cout << "Inputs:" << endl;
	for (std::vector<InputsOutputs>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
		std::cout << it->getName() << " : " << it->getDataType() << " : " << it->getBitWidth() << std::endl;
	}
	std::cout << "Outputs:" << endl;
	for (std::vector<InputsOutputs>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
		std::cout << it->getName() << " : " << it->getDataType() << " : " << it->getBitWidth() << std::endl;
	}
	std::cout << "Variables:" << endl;
	for (std::vector<InputsOutputs>::iterator it = variables.begin(); it != variables.end(); ++it) {
		std::cout << it->getName() << " : " << it->getDataType() << " : " << it->getBitWidth() << std::endl;
	}
	std::cout << "......................................" << std::endl;
	for (std::vector<string>::iterator it = rawFileStrings.begin(); it != rawFileStrings.end(); ++it) {
		std::cout << *it << std::endl;
	}
	return 0;
}