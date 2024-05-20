#ifndef SCREENCOMPONENT_H
#define SCREENCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class ScreenComponent final : public BaseComponent, public Subject
	{
	public:
		void Update() override;

		explicit ScreenComponent(GameObject* pOwner, float maxTime);
		ScreenComponent(const ScreenComponent& other) = delete;
		ScreenComponent(ScreenComponent&& other) = delete;

	private:
		float m_maxWaitTime;
		float m_accumulatedTime{};
	};
}
#endif