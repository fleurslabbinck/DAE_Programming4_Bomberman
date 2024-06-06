#include "PvpPlayState.h"

#include "../BombermanManager.h"
#include "PvpScreenState.h"
#include "WinnerScreenState.h"

namespace dae
{
	void PvpPlayState::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::RESET_LEVEL:
			BombermanManager::GetInstance().LoseHealth();
			m_state = std::make_unique<PvpScreenState>();
			break;
		case GameEvent::STAGE_WON:
			BombermanManager::GetInstance().SetPvpWinner("BOMBERMAN");
			BombermanManager::GetInstance().ResetHealth();
			m_state = std::make_unique<WinnerScreenState>();
			break;
		case GameEvent::GAME_OVER:
			BombermanManager::GetInstance().SetPvpWinner("BALLOOM");
			BombermanManager::GetInstance().ResetHealth();
			m_state = std::make_unique<WinnerScreenState>();
			break;
		default:
			m_state = nullptr;
			break;
		}
	}

	void PvpPlayState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Pvp));
	}
}