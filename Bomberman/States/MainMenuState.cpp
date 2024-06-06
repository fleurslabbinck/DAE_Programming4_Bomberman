#include "MainMenuState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "StageScreenState.h"
#include "PvpScreenState.h"
#include "HighScoreState.h"

namespace dae
{
	void MainMenuState::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::START_SINGLE:
			BombermanManager::GetInstance().SetTotalPlayers(m_singlePlayer);
			m_state = std::make_unique<StageScreenState>();
			break;
		case GameEvent::START_COOP:
			BombermanManager::GetInstance().SetTotalPlayers(m_multiPlayer);
			m_state = std::make_unique<StageScreenState>();
			break;
		case GameEvent::START_PVP:
			BombermanManager::GetInstance().SetTotalPlayers(m_multiPlayer);
			m_state = std::make_unique<PvpScreenState>();
			break;
		case GameEvent::HIGHSCORES:
			m_state = std::make_unique<HighScoreState>();
			break;
		default:
			m_state = nullptr;
			break;
		}
	}

	void MainMenuState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Menu));
	}
}