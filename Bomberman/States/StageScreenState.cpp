#include "StageScreenState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "SinglePlayState.h"

namespace dae
{
	void StageScreenState::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE))
		{
			m_state = std::make_unique<SinglePlayState>();
		}
	}

	void StageScreenState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::StageScreen));
	}
}