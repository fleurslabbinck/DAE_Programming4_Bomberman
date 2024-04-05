#ifndef MINIGINUTIL_H
#define MINIGINUTIL_H

#include <glm/glm.hpp>

constexpr int WINDOW_SCALE = 4;

//window dimensions
constexpr int WINDOW_WIDTH = 256 * WINDOW_SCALE;
constexpr int WINDOW_HEIGHT = 240 * WINDOW_SCALE;
constexpr int GRIDCELL = 16 * WINDOW_SCALE;

namespace dae
{
	struct Cell {
		int size{ GRIDCELL };
		glm::vec2 startPos;
		glm::vec2 center;
	};
}

constexpr int GRID_COLS = 31;
constexpr int GRID_ROWS = 13;

#endif