#include "InfoCommand.h"

#include <iostream>
#include <iomanip>

namespace dae
{
	//---------------------------------
	// INFO
	//---------------------------------
	void InfoCommand::Execute()
	{
		std::cout << "\n------------INFO------------\n";
		std::cout << "BOMBERMAN CONTROLS\n";
		std::cout << std::left << std::setw(12) << "UP: " << "DPAD UP / ARROW UP\n";
		std::cout << std::left << std::setw(12) << "LEFT: " << "DPAD LEFT / ARROW LEFT\n";
		std::cout << std::left << std::setw(12) << "DOWN: " << "DPAD DOWN / ARROW DOWN\n";
		std::cout << std::left << std::setw(12) << "RIGHT: " << "DPAD RIGHT / ARROW RIGHT\n";
		std::cout << std::left << std::setw(12) << "DROP BOMB: " << "A / X\n";
		std::cout << std::left << std::setw(12) << "DETONATE: " << "B / Z\n\n";

		std::cout << "BALLOOM / BOMBERMAN2 CONTROLS\n";
		std::cout << std::left << std::setw(12) << "UP: " << "W\n";
		std::cout << std::left << std::setw(12) << "LEFT: " << "A\n";
		std::cout << std::left << std::setw(12) << "DOWN: " << "S\n";
		std::cout << std::left << std::setw(12) << "RIGHT: " << "D\n";
		std::cout << std::left << std::setw(12) << "DROP BOMB: " << "P\n";
		std::cout << std::left << std::setw(12) << "DETONATE: " << "O\n\n";

		std::cout << "MENU CONTROLS\n";
		std::cout << std::left << std::setw(12) << "UP: " << "DPAD UP / ARROW UP\n";
		std::cout << std::left << std::setw(12) << "DOWN: " << "DPAD DOWN / ARROW DOWN\n";
		std::cout << std::left << std::setw(12) << "CONTINUE: " << "A / ENTER\n";
		std::cout << std::left << std::setw(12) << "BACK: " << "B / BACKSPACE\n";
	}
}