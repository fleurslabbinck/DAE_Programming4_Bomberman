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
}

namespace scenes
{
	enum class Scenes {
		Menu,
		EnterName,
		StageScreen,
		Normal,
		GameOverScreen,
		HighScore,
		PvpScreen,
		Pvp,
		WinnerScreen,
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
		Bombs,
		Fire,
		Detonator
	};
}

namespace sound
{
	enum class SoundId {
		HorizontalStep,
		VerticalStep,
		DeathSound,
		DeathTune,
		PlaceBomb,
		Explosion,
		StartLevel,
	};
}

enum class GameEvent {
	ENTITYOVERLAP,

	PLAYER_DEATH,
	ENEMY_DEATH,
	PLAYER_EXIT,
	PLAYER_POWERUP,

	BOMBS,
	FIRE,
	DETONATOR,

	HEALTH_CHANGED,
	SCORE_CHANGED,
	PLAYER_RESPAWN,

	START_SINGLE,
	START_COOP,
	START_PVP,
	HIGHSCORES,

	STAGE_WON,
	RESET_LEVEL,
	GAME_OVER,

	CONTINUE,
	BACK,
	UP,
	DOWN,
};

namespace powerUps
{
	struct PowerUpState {
		uint8_t maxBombs{ 2 };
		uint8_t fire{ 1 };
		bool canDetonate{ false };
	};
}

#endif