#include "PlayState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "MainMenuState.h"
#include "HighScoreState.h"

namespace dae
{
	GameState* PlayState::HandleGame(Input input)
	{
		switch (static_cast<gameInput::GameInput>(input))
		{
		case gameInput::GameInput::NextLevel:
			NextLevel();
			return new PlayState();
			break;
		case gameInput::GameInput::LevelReset:
			ResetLevel();
			BombermanManager::GetInstance().LoseHealth();
			return new PlayState();
			break;
		case gameInput::GameInput::GameWon:
			BombermanManager::GetInstance().ResetHealth();
			ResetLevel();
			return new HighScoreState();
			break;
		case gameInput::GameInput::GameOver:
			BombermanManager::GetInstance().ResetHealth();
			ResetLevel();
			return new HighScoreState();
			break;
		default:
			return nullptr;
			break;
		}
	}

	void PlayState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Level), m_currentLevel);
	}

	void PlayState::NextLevel()
	{
		if (m_currentLevel < m_maxLevels - 1) ++m_currentLevel;
	}

	void PlayState::ResetLevel()
	{
		m_currentLevel = 0;
	}
}