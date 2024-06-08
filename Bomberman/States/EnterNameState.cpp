#include "EnterNameState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "StageScreenState.h"

namespace dae
{
	void EnterNameState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE))
		{
			m_state = std::make_unique<StageScreenState>();
		}
	}

	void EnterNameState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::EnterName));
	}
}