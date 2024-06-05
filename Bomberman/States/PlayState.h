#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Game/GameState.h"
#include "../BombermanUtil.h"

namespace dae
{
	class PlayState final : public GameState
	{
	public:
		PlayState() = default;
		PlayState(const PlayState& other) = delete;
		PlayState(PlayState&& other) = delete;
		PlayState& operator=(const PlayState& other) = delete;
		PlayState& operator=(PlayState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;

	private:
		powerUps::PowerUpState m_powerUpState{};

		void ResetLevel();
	};
}
#endif