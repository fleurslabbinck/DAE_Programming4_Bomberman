#ifndef EXITCOMPONENT_H
#define EXITCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class GameObject;
	class ColliderComponent;
	class RenderComponent;

	//---------------------------------
	//EXITCOMPONENT
	//---------------------------------
	class ExitComponent : public BaseComponent, public Observer, public Subject
	{
	public:
		void OnNotify(Event event, GameObject* gameObject) override;

		explicit ExitComponent(GameObject* pOwner, uint8_t totalEnemies);
		~ExitComponent() override;
		ExitComponent(const ExitComponent& other) = delete;
		ExitComponent(ExitComponent&& other) = delete;

	private:
		uint8_t m_totalEnemies;
		uint8_t m_killedEnemies{};

		std::unique_ptr<ColliderComponent> m_colliderComponent;
		std::unique_ptr<RenderComponent> m_renderComponent;
	};
}
#endif