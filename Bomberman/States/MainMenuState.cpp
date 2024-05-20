#include "MainMenuState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "PlayState.h"

namespace dae
{
	void MainMenuState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE)) m_state = std::make_unique<PlayState>();
		else m_state = nullptr;
	}

	void MainMenuState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Menu));
	}
}