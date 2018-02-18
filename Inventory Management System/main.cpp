
#include <iostream>

#include "InventorySystem.h"

int main(int argc, char** argv)
{
	InventorySystem is;

	if (argc == 2) {
		is.init(argv[1]);
	}
	else {
		std::cout << "Invalid parameters!\nProper usage: IMS <CompanyName>\nIf the company does not exist you will be asked if it should be created or not.";
	}

	return 0;
}