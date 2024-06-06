#ifndef PVPPLAYSTATE_H
#define PVPPLAYSTATE_H

#include "Game/GameState.h"
#include "../BombermanUtil.h"

namespace dae
{
	class PvpPlayState final : public GameState
	{
	public:
		PvpPlayState() = default;
		PvpPlayState(const PvpPlayState& other) = delete;
		PvpPlayState(PvpPlayState&& other) = delete;
		PvpPlayState& operator=(const PvpPlayState& other) = delete;
		PvpPlayState& operator=(PvpPlayState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif