#include "InputsOutputs.h"
#include <string>



InputsOutputs::InputsOutputs(std::string sName, std::string sDataString, std::string strBitWidth)
{
	name = sName;
	dataType = sDataString;
	sBitWidth = strBitWidth;

	return;
}

std::string InputsOutputs::getName(void)
{
	return name;
}

std::string InputsOutputs::getBitWidth()
{
	return sBitWidth;
}


std::string InputsOutputs::getDataType(void)
{
	return dataType;
}


void InputsOutputs::printPin()
{
}

void InputsOutputs::generateInputsOutputs(std::vector<string>* strVector, std::vector<InputsOutputs>* inputs, std::vector<InputsOutputs>* outputs)
{
	std::vector<string> tok;
	std::vector<string> sNames;
	std::string sType;
	std::string sDataString;
	std::string bitSubStr;
	string bitWidth;

	//std::vector<InputsOutputs> inputs;

	if (strVector->size() == 0) {
		std::cout << "Error parsing file (V_Module.cpp)" << std::endl;
		exit(1);
		//erFlag = true;
		//return false;
	}

	for (std::vector<string>::iterator it = strVector->begin(); it != strVector->end(); ++it) {
		//For each string in the vector which the file was loaded in to
		tok = Parser::splitByWhitespace(*it);//break the line up by whitespace

		if (tok.size() == 0) continue;
		//sType = tok.at(0);//the first token should be input, output, wire etc

		if (tok.at(0) == "input" || tok.at(0) == "output") {
			//newCon = new InputsOutputs("input");
			

			sDataString = tok.at(1); //The second item is always the bitwidth string

			if (sDataString.substr(0, 4) == "UInt") {
				bitSubStr = sDataString.substr(4, sDataString.size());
				//	sgn = UNSIGNED;
				sType = "UInt";
				std::istringstream iss(bitSubStr);
				if (!(iss >> bitWidth)) {
					std::cout << "Invalid bit string" << std::endl;
					exit(1);
				}
			}
			else if (sDataString.substr(0, 3) == "Int") {
				sType = "Int";
				bitSubStr = sDataString.substr(3, sDataString.size());
				std::istringstream iss(bitSubStr);
				//sDataString.substr(0, 3)
				if (!(iss >> bitWidth)) std::cout << "Invalid bit string" << std::endl;
				//if (bitWidth > 1) sgn = SIGNED;
			}
			else {
				std::cout << "Error! Invalid data type:" << sDataString << std::endl;
				exit(1);
			}

			for (std::vector<string>::iterator it = tok.begin() + 2; it != tok.end(); ++it) {
				//std::cout << *it;
				sNames.push_back(*it);//Everything after the bitwidth string is a new pin, so we're making a vector of pin names
				//sNames.insert(sNames.begin(), *it);
			}


			for (std::vector<string>::iterator it = sNames.begin(); it != sNames.end(); ++it) {
				//loop through the vector of pin names, and make a new V_Pin object for it
				InputsOutputs *newInputsOutputs = new InputsOutputs(*it, sType, bitWidth);

				if (tok.at(0) == "input") {
					if (inputs->size() == 0) inputs->push_back(*newInputsOutputs);//If the vector is empty, just put this in there.
					else inputs->push_back(*newInputsOutputs);// pins.insert(pins.begin(), newPin);//They should load in order on their own
				}
				else if (tok.at(0) == "output") {
					if (outputs->size() == 0) outputs->push_back(*newInputsOutputs);//If the vector is empty, just put this in there.
					else outputs->push_back(*newInputsOutputs);// pins.insert(pins.begin(), newPin);//They should load in order on their own
				}
				
			}
			sNames.clear();//After making all of the pins with this type and bitwidth, empty the name vector and move to the next type
		}

	}


			


}
