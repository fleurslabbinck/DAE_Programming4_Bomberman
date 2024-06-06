#ifndef SINGLEPLAYSTATE_H
#define SINGLEPLAYSTATE_H

#include "Game/GameState.h"
#include "../BombermanUtil.h"

namespace dae
{
	class StagePlayState final : public GameState
	{
	public:
		StagePlayState() = default;
		StagePlayState(const StagePlayState& other) = delete;
		StagePlayState(StagePlayState&& other) = delete;
		StagePlayState& operator=(const StagePlayState& other) = delete;
		StagePlayState& operator=(StagePlayState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;

	private:
		powerUps::PowerUpState m_powerUpState{};

		void ResetLevel();
	};
}
#endif