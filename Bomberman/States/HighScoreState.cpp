#include "HighScoreState.h"

#include "MainMenuState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "MainMenuState.h"

namespace dae
{
	GameState* HighScoreState::HandleGame(Input input)
	{
		if (input == static_cast<int>(gameInput::GameInput::Back)) return new MainMenuState();

		return nullptr;
	}

	void HighScoreState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::HighScore));
	}
}