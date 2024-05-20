#ifndef GAMEOVERSCREENSTATE_H
#define GAMEOVERSCREENSTATE_H

#include "Game/GameState.h"

namespace dae
{
	class GameOverScreenState final : public GameState
	{
	public:
		GameOverScreenState() = default;
		GameOverScreenState(const GameOverScreenState& other) = delete;
		GameOverScreenState(GameOverScreenState&& other) = delete;
		GameOverScreenState& operator=(const GameOverScreenState& other) = delete;
		GameOverScreenState& operator=(GameOverScreenState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif