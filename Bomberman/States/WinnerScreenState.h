#ifndef WINNERSCREENSTATE_H
#define WINNERSCREENSTATE_H

#include "Game/GameState.h"

namespace dae
{
	class WinnerScreenState final : public GameState
	{
	public:
		WinnerScreenState() = default;
		WinnerScreenState(const WinnerScreenState& other) = delete;
		WinnerScreenState(WinnerScreenState&& other) = delete;
		WinnerScreenState& operator=(const WinnerScreenState& other) = delete;
		WinnerScreenState& operator=(WinnerScreenState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif