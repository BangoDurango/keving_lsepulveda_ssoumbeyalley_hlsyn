
#include "Parser.h"

//using namespace std;
void Parser::parseFile(char* inFileStr, std::vector<string>* destVector) {

	std::string line;
	std::ifstream inFile(inFileStr, std::ifstream::in);


	if (inFile.is_open() && inFile.good()) {
	//	std::cout << "File Opened!" << std::endl;
	}
	else {
		int n;
		std::cout << "Unable to open input file." << std::endl;// << "Enter a number to exit FIXME" << std::endl;
		//std::cin >> n;
		exit(1);
		//return std::vector<std::string>();
	}
	std::string str;
	while (std::getline(inFile, line)) {

		if (line.find_first_not_of(' ') != std::string::npos) {
			str = trim(line);
			destVector->push_back(str);
		}


	}


	inFile.close();

}
std::string Parser::trim(string& str)
{
	std::string sTMP;
	sTMP = str;
	sTMP.erase(std::remove(sTMP.begin(), sTMP.end(), '\t'), sTMP.end());
	size_t first = sTMP.find_first_not_of(' ');
	size_t last = sTMP.find_last_not_of(' ');
	return sTMP.substr(first, (last - first + 1));
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