
#include <iostream>
#include "bonus.hpp"

int main(int argc, char *argv[])
{
	Bonus::loadAll("rc/");
	Bonus::freeAll();
	return 0;
}

