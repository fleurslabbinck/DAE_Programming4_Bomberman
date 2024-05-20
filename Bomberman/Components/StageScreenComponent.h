#ifndef STAGESCREENCOMPONENT_H
#define STAGESCREENCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class StageScreenComponent final : public BaseComponent, public Subject
	{
	public:
		void Update() override;

		explicit StageScreenComponent(GameObject* pOwner, float maxTime);
		StageScreenComponent(const StageScreenComponent& other) = delete;
		StageScreenComponent(StageScreenComponent&& other) = delete;

	private:
		float m_maxWaitTime;
		float m_accumulatedTime{};
	};
}
#endif