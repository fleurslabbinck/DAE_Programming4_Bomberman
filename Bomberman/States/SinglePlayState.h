#ifndef SINGLEPLAYSTATE_H
#define SINGLEPLAYSTATE_H

#include "Game/GameState.h"
#include "../BombermanUtil.h"

namespace dae
{
	class SinglePlayState final : public GameState
	{
	public:
		SinglePlayState() = default;
		SinglePlayState(const SinglePlayState& other) = delete;
		SinglePlayState(SinglePlayState&& other) = delete;
		SinglePlayState& operator=(const SinglePlayState& other) = delete;
		SinglePlayState& operator=(SinglePlayState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;

	private:
		powerUps::PowerUpState m_powerUpState{};

		void ResetLevel();
	};
}
#endif