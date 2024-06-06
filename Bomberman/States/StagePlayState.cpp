#include "StagePlayState.h"

#include "../BombermanManager.h"
#include "HighScoreState.h"
#include "StageScreenState.h"
#include "GameOverScreenState.h"

namespace dae
{
	void StagePlayState::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::STAGE_WON:
			if (BombermanManager::GetInstance().GetCurrentLevel() < BombermanManager::GetInstance().GetMaxLevels())
			{
				BombermanManager::GetInstance().NextLevel(m_powerUpState);
				m_state = std::make_unique<StageScreenState>();
			}
			else
			{
				ResetLevel();
				m_state = std::make_unique<HighScoreState>();
			}
			break;
		case GameEvent::RESET_LEVEL:
			BombermanManager::GetInstance().LoseHealth();
			m_state = std::make_unique<StageScreenState>();
			break;
		case GameEvent::GAME_OVER:
			ResetLevel();
			m_state = std::make_unique<GameOverScreenState>();
			break;
		case GameEvent::BOMBS:
			++m_powerUpState.maxBombs;
			break;
		case GameEvent::FIRE:
			++m_powerUpState.fire;
			break;
		case GameEvent::DETONATOR:
			m_powerUpState.canDetonate = true;
			break;
		default:
			m_state = nullptr;
			break;
		}
	}

	void StagePlayState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::SinglePlayer));
	}

	void StagePlayState::ResetLevel()
	{
		BombermanManager::GetInstance().ResetLevel();
		BombermanManager::GetInstance().ResetHealth();
	}
}