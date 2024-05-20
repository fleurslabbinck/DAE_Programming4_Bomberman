#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <memory>

#include "../Observers/Observer.h"

namespace dae
{
	class GameState : public Observer
	{
	public:
		GameState() = default;
		virtual ~GameState() = default;
		GameState(const GameState& other) = delete;
		GameState(GameState&& other) = delete;
		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) = delete;

		virtual void OnNotify(Event, GameObject*) override {};
		virtual std::unique_ptr<GameState> HandleGame() { return nullptr; };
		virtual void OnEnter() const {};

	protected:
		std::unique_ptr<GameState> m_state{ nullptr };
	};
}
#endif