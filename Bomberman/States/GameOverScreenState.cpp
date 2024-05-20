#include "GameOverScreenState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "HighScoreState.h"

namespace dae
{
	void GameOverScreenState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE))
		{
			m_state = std::make_unique<HighScoreState>();
		}
	}

	void GameOverScreenState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::GameOverScreen));
	}
}