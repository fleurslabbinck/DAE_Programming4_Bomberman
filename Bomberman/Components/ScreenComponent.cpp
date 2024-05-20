#include "ScreenComponent.h"

#include "TimeManager.h"
#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	//SCREENCOMPONENT
	//---------------------------------
	ScreenComponent::ScreenComponent(GameObject* pOwner, float maxTime)
		: BaseComponent(pOwner), m_maxWaitTime{ maxTime }
	{}

	void ScreenComponent::Update()
	{
		const float timePassed{ TimeManager::GetInstance().GetDeltaTime() };

		m_accumulatedTime += timePassed;

		if (m_accumulatedTime >= m_maxWaitTime)
		{
			Notify(static_cast<int>(GameEvent::CONTINUE), nullptr);
		}
	}
}