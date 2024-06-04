#include "PlayState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "HighScoreState.h"
#include "StageScreenState.h"
#include "GameOverScreenState.h"

namespace dae
{
	void PlayState::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::NEXT_LEVEL:
			NextLevel();
			m_state = std::make_unique<StageScreenState>();
			break;
		case GameEvent::RESET_LEVEL:
			BombermanManager::GetInstance().LoseHealth();
			m_state = std::make_unique<StageScreenState>();
			break;
		case GameEvent::GAME_WON:
			ResetLevel();
			m_state = std::make_unique<HighScoreState>();
			break;
		case GameEvent::GAME_OVER:
			ResetLevel();
			m_state = std::make_unique<GameOverScreenState>();
			break;
		default:
			m_state = nullptr;
			break;
		}
	}

	void PlayState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Level));
	}


	void PlayState::NextLevel()
	{
		BombermanManager::GetInstance().NextLevel();
	}

	void PlayState::ResetLevel()
	{
		BombermanManager::GetInstance().ResetLevel();
		BombermanManager::GetInstance().ResetHealth();
	}
}