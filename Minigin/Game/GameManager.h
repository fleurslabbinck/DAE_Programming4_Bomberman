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
		virtual ~GameManager()
		{
			delete m_state;
			m_state = nullptr;
		}
		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		virtual void HandleGame(Input input) = 0;

	protected:
		GameState* m_state{};
	};
}
#endif