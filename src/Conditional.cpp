#include "Conditional.h"
Conditional::Conditional() {

}
Conditional::Conditional(std::string Arg) {
	sArg = Arg;
}
Conditional::Conditional(std::string Arg, Func * inPrev)
{
	sArg = Arg;
	fPrev = inPrev;

}

void Conditional::setNextIfTrue(Func * f)
{
	if (cNextIfTrue != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	fNextIfTrue = f;
}

void Conditional::setNextIfTrue(Conditional * c)
{
	if (fNextIfTrue != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	cNextIfTrue = c;
}

void Conditional::setNextIfFalse(Func * f)
{
	if (cNextIfFalse != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	fNextIfFalse = f;
}

void Conditional::setNextIfFalse(Conditional * c)
{
	if (fNextIfFalse != NULL) {
		std::cout << "warning: Conditional already has both function and condition assigned to next." << std::endl;
	}
	cNextIfFalse = c;
}
