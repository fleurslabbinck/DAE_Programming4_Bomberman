#include "PvpScreenState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "PvpPlayState.h"

namespace dae
{
	void PvpScreenState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE))
		{
			m_state = std::make_unique<PvpPlayState>();
		}
	}

	void PvpScreenState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::PvpScreen));
	}
}