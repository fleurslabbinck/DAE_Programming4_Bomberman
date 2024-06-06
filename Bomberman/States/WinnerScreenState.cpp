#include "WinnerScreenState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "MainMenuState.h"

namespace dae
{
	void WinnerScreenState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE))
		{
			m_state = std::make_unique<MainMenuState>();
		}
	}

	void WinnerScreenState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::WinnerScreen));
	}
}