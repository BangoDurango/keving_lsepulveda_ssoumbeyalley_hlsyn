#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define INPUT "input"
#define OUTPUT "output"
#define VARIABLE "variable"
#define REGISTER "register"
#define REG "reg"
#define INVALID "invalid"
#define ADDSUB "addsub"
#define PLUS "+"
#define MINUS "-"
#define MULT "*"
#define LOGIC "logic"
#define LT "<"
#define GT ">"
#define ET "++"
#define SL "<<"
#define SR ">>"
#define MUX "?"
#define INOP "INOP"
#define ONOP "NOP"
#define IF "if ("
#define ELSE "else {"
#define SIGNED true
#define UNSIGNED false


#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include<string>
#include <vector>





using namespace std;

enum BlockType { CONDITIONAL, FUNCTION };