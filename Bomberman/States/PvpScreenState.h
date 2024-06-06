#ifndef PVPSCREENSTATE_H
#define PVPSCREENSTATE_H

#include "Game/GameState.h"

namespace dae
{
	class PvpScreenState final : public GameState
	{
	public:
		PvpScreenState() = default;
		PvpScreenState(const PvpScreenState& other) = delete;
		PvpScreenState(PvpScreenState&& other) = delete;
		PvpScreenState& operator=(const PvpScreenState& other) = delete;
		PvpScreenState& operator=(PvpScreenState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif