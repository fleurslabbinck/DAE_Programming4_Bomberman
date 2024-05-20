#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Game/GameState.h"

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
		void NextLevel();
		void ResetLevel();
	};
}
#endif