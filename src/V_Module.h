#pragma once
/***
Author: Kevin Gilliam
NetID: keving
Assignment #: 2
File: V_Module.h
*/
#ifndef V_MODULE_H
#define V_MODULE_H


#include<string>
#include <vector>
#include<iostream>
#include <fstream>
#include <sstream>
#include<algorithm>
#include "V_Pin.h"
#include "Parser.h"
#include "V_Component.h"
#include "main.h"

class V_Module {

public:
	~V_Module();//to delete strings
	
//	void addPin(std::string sName, std::string sType, std::string sbitWidth);
	void printLines(void);

	
	void generatePins();//Uses the lines from the text file to generate a vector of V_Pin objects
	void generateComponents();//For Luis - see end of V_Module.cpp
	
	std::vector<string>* getFileStringVector();

	void generateVerilogFile(char* outFileStr);
	//V_Pin* getPinByName(std::string sName);
	string debugName;
private:
	std::vector<V_Pin*> pins;
	std::vector<V_Component*> comps;
	std::vector<string> rawFileStrings;
	std::vector<string>::iterator MOF; //MOF stands for Middle of File - the generatePins() function sets this at the 
	//when it has reached the end of the input/output declarations, so the generateComponents() can start from this point
	V_Pin* getPinByName(std::string sName);
};


#endif// V_MODULE_H