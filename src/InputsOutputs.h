#pragma once
/***
Author: Kevin Gilliam
NetID: keving
Assignment #: 3

*/
#ifndef INPUT_H
#define INPUT_H

#include<string>
#include <vector>
#include<iostream>
#include <sstream>
#include "Parser.h"
#include "main.h"
using namespace std;

class InputsOutputs {

public:

	//InputsOutputs();
	InputsOutputs(std::string sName, std::string sDataString, std::string sBitWidth);
	//InputsOutputs(std::string s);
	std::string getName(void);
	std::string getBitWidth();
	std::string getDataType(void);
	//unsigned int getBitWidth(void);
	void printPin();
	//std::vector<InputsOutputs> generateInputsOutputss(std::string inputString);
	//static bool CheckType(std::string);
	static  void generateInputsOutputs(std::vector<string>* strVector, std::vector<InputsOutputs>* inputs, std::vector<InputsOutputs>* outputs, std::vector<InputsOutputs>* variables);

private:

	//unsigned int bitWidth;
	std::string sBitWidth;
	std::string name;
	std::string inputType;
	std::string dataType;

};
#endif //CONNECTION_H