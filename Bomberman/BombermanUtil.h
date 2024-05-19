#ifndef BOMBERMAN_UTIL_H
#define BOMBERMAN_UTIL_H

#include <string>
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

namespace scenes
{
	enum class Scenes {
		Menu,
		Level,
		HighScore,
	};
}

namespace gameInput
{
	enum class GameInput {
		Continue,
		Back,
		NextLevel,
		LevelReset,
		GameWon,
		GameOver,
	};
}

namespace entities
{
	enum class EntityType {
		Bomberman,
		Bomb,
		Explosion,
		Balloom,
		Oneal,
		Doll,
		Minvo,
		Brick,
		ExitWay,
		PowerUp,
	};
}

namespace sound
{
	enum class SoundId {
		DeathSound,
		DeathTune,
	};
}

enum class GameEvent {
	ENTITYOVERLAP,
	PLAYER_DEATH,
	ENEMY_DEATH,
	BRICK_DEATH,
	HEALTH_CHANGED,
	SCORE_CHANGED,
	PLAYER_RESPAWN,
	GAME_OVER,
};

#endif