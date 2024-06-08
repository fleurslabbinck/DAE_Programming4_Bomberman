#ifndef ENTERNAMESTATE_H
#define ENTERNAMESTATE_H

#include "Game/GameState.h"

namespace dae
{
	class EnterNameState final : public GameState
	{
	public:
		EnterNameState() = default;
		EnterNameState(const EnterNameState& other) = delete;
		EnterNameState(EnterNameState&& other) = delete;
		EnterNameState& operator=(const EnterNameState& other) = delete;
		EnterNameState& operator=(EnterNameState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif