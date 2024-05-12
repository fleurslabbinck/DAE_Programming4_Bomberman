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

		GameState* HandleGame(Input input) override;
		void OnEnter() const override;

	private:
		const int m_maxLevels{ 3 };
		inline static int m_currentLevel{};

		void NextLevel();
		void ResetLevel();
	};
}
#endif