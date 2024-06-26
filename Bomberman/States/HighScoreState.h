#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include "Game/GameState.h"

namespace dae
{
	class HighScoreState final : public GameState
	{
	public:
		HighScoreState() = default;
		HighScoreState(const HighScoreState& other) = delete;
		HighScoreState(HighScoreState&& other) = delete;
		HighScoreState& operator=(const HighScoreState& other) = delete;
		HighScoreState& operator=(HighScoreState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;
	};
}
#endif