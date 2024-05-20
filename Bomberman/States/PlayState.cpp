#include "PlayState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "MainMenuState.h"
#include "HighScoreState.h"

namespace dae
{
	void PlayState::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::NEXT_LEVEL:
			NextLevel();
			m_state = std::make_unique<PlayState>();
			break;
		case GameEvent::RESET_LEVEL:
			ResetLevel();
			BombermanManager::GetInstance().LoseHealth();
			m_state = std::make_unique<PlayState>();
			break;
		case GameEvent::GAME_WON:
			BombermanManager::GetInstance().ResetHealth();
			ResetLevel();
			m_state = std::make_unique<HighScoreState>();
			break;
		case GameEvent::GAME_OVER:
			BombermanManager::GetInstance().ResetHealth();
			ResetLevel();
			m_state = std::make_unique<HighScoreState>();
			break;
		default:
			m_state = nullptr;
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