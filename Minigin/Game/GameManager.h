#ifndef GAME_H
#define GAME_H

#include "GameState.h"

namespace dae
{
	using GameScene = int;

	class GameManager
	{
	public:
		GameManager() = default;
		virtual ~GameManager() = default;
		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		virtual void HandleGame() = 0;

	protected:
		std::unique_ptr<GameState> m_state{};
	};
}
#endif