#include "StageScreenComponent.h"

#include "TimeManager.h"
#include "../BombermanUtil.h"

namespace dae
{
	StageScreenComponent::StageScreenComponent(GameObject* pOwner, float maxTime)
		: BaseComponent(pOwner), m_maxWaitTime{ maxTime }
	{}

	void StageScreenComponent::Update()
	{
		const float timePassed{ TimeManager::GetInstance().GetDeltaTime() };

		m_accumulatedTime += timePassed;

		if (m_accumulatedTime >= m_maxWaitTime)
		{
			Notify(static_cast<int>(GameEvent::START_LEVEL), nullptr);
		}
	}
}