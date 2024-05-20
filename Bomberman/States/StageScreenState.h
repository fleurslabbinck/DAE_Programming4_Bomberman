#ifndef STAGESCREENSTATE_H
#define STAGESCREENSTATE_H

#include "Game/GameState.h"

namespace dae
{
	class StageScreenState final : public GameState
	{
	public:
		StageScreenState() = default;
		StageScreenState(const StageScreenState& other) = delete;
		StageScreenState(StageScreenState&& other) = delete;
		StageScreenState& operator=(const StageScreenState& other) = delete;
		StageScreenState& operator=(StageScreenState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		std::unique_ptr<GameState> HandleGame() override { return std::move(m_state); }
		void OnEnter() const override;
	};
}
#endif