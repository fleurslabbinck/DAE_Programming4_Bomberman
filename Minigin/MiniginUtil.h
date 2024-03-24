#ifndef MINIGINUTIL_H
#define MINIGINUTIL_H

#include <glm/glm.hpp>

//window dimensions
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 960;
constexpr int GRIDCELL = 64;

namespace dae
{
	struct Cell {
		int size{ GRIDCELL };
		glm::vec2 startPos;
		glm::vec2 center;
	};
}

#endif
