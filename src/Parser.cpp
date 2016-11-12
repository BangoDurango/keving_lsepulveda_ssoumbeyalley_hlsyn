
#include "Parser.h"

//using namespace std;
void Parser::parseFile(char* inFileStr, std::vector<string>* destVector) {

	std::string line;
	std::ifstream inFile(inFileStr, std::ifstream::in);


	if (inFile.is_open() && inFile.good()) {
	//	std::cout << "File Opened!" << std::endl;
	}
	else {
		std::cout << "Unable to open input file." << std::endl;
		exit(1);
		//return std::vector<std::string>();
	}

	while (std::getline(inFile, line)) {

		if (line != "") {
			destVector->push_back(line);
		}


	}


	inFile.close();

}

vector<std::string> Parser::splitByWhitespace(std::string str) {

	string buf; // Have a buffer string
	stringstream ss(str); // Insert the string into a stream

	vector<string> tokens; // Create vector to hold our words
//	bool test;
	while (ss >> buf) {
		buf.erase(std::remove(buf.begin(), buf.end(), ','), buf.end());
		//std::cout << buf << "***";
		//if (buf == "b") { break; }
		
		//test = buf.find("\\");
	//	std::cout << buf << "....." << test << endl;
	
		if (buf.find("//") == string::npos) {
			tokens.push_back(buf);
		//	
		}
		else { 
		//	std::cout << "found!....." << buf << std::endl;
			break; 
		}
			

	}
	return tokens;

}

std::string Parser::getModuleName(std::string filePath) {
	std::string name;
	std::string path;
	std::size_t found = filePath.find_last_of("/\\");


	//name = filePath.substr(found1 + 1).
	
	name = filePath.substr(found + 1);
	found = name.find_last_of(".");
	//std::cout << name;
	name = name.substr(0, found);


	return name;
	//std::cout << fileName << std::endl;
	//std::cout << filePath << std::endl;

}
std::string Parser::getFilePath(std::string filePath) {
	std::string name;
	std::string path;
	//string path2;
	std::size_t found = filePath.find_last_of("/\\");

	path = filePath.substr(0, found);
	//std::cout << path << endl;
	return path;
	//std::cout << fileName << std::endl;
	//std::cout << filePath << std::endl;

}