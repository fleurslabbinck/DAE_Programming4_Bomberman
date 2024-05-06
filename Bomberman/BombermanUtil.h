#ifndef BOMBERMAN_UTIL_H
#define BOMBERMAN_UTIL_H

#include <glm/glm.hpp>

namespace constants
{
	//window dimensions
	constexpr int WINDOW_SCALE = 2;
	constexpr int WINDOW_WIDTH = 256 /** WINDOW_SCALE*/;
	constexpr int WINDOW_HEIGHT = 240 /** WINDOW_SCALE*/;

	//grid
	constexpr int GRIDCELL = 16 /** WINDOW_SCALE*/;
	constexpr int GRID_COLS = 31;
	constexpr int GRID_ROWS = 13;
}

namespace sound
{
	enum class SoundId {
		DeathSound,
		DeathTune,
	};
}

enum class GameEvent {
	PLAYER_HIT,
	ENEMY_HIT,
	WALL_HIT,
	PLAYER_DEATH,
	ENEMY_DEATH,
	WALL_DEATH,
	HEALTH_CHANGED,
	SCORE_CHANGED,
	PLAYER_RESPAWN,
	GAME_OVER,
};

#endif