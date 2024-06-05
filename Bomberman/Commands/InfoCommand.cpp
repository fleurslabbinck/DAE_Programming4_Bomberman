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
		std::cout << std::left << std::setw(10) << "UP: " << "DPAD UP / ARROW UP\n";
		std::cout << std::left << std::setw(10) << "LEFT: " << "DPAD LEFT / ARROW LEFT\n";
		std::cout << std::left << std::setw(10) << "DOWN: " << "DPAD DOWN / ARROW DOWN\n";
		std::cout << std::left << std::setw(10) << "RIGHT: " << "DPAD RIGHT / ARROW RIGHT\n\n";

		std::cout << "BALLOOM CONTROLS\n";
		std::cout << std::left << std::setw(10) << "UP: " << "W\n";
		std::cout << std::left << std::setw(10) << "LEFT: " << "A\n";
		std::cout << std::left << std::setw(10) << "DOWN: " << "S\n";
		std::cout << std::left << std::setw(10) << "RIGHT: " << "D\n\n";

		std::cout << "MENU CONTROLS\n";
		std::cout << std::left << std::setw(10) << "UP: " << "DPAD UP / ARROW UP\n";
		std::cout << std::left << std::setw(10) << "DOWN: " << "DPAD DOWN / ARROW DOWN\n";
		std::cout << std::left << std::setw(10) << "CONTINUE: " << "A / ENTER\n";
		std::cout << std::left << std::setw(10) << "BACK: " << "B / BACKSPACE\n";

		std::cout << "\n(high score sceen only possible through game over, 2 deaths needed)\n";
	}
}