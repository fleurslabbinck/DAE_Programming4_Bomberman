#include "HighScoreState.h"

#include "MainMenuState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "MainMenuState.h"

namespace dae
{
	void HighScoreState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::BACK)) m_state = std::make_unique<MainMenuState>();
	}

	void HighScoreState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::HighScore));
	}
}