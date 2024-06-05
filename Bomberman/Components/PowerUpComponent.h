#ifndef POWERUPCOMPONENT_H
#define POWERUPCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"
#include "../BombermanUtil.h"

namespace dae
{
	class GameObject;
	class ColliderComponent;
	class RenderComponent;

	//---------------------------------
	//POWERUPCOMPONENT
	//---------------------------------
	class PowerUpComponent : public BaseComponent, public Observer, public Subject
	{
	public:
		void OnNotify(Event event, GameObject* gameObject) override;

		explicit PowerUpComponent(GameObject* pOwner, entities::EntityType powerUpType);
		~PowerUpComponent() override;
		PowerUpComponent(const PowerUpComponent& other) = delete;
		PowerUpComponent(PowerUpComponent&& other) = delete;

	private:
		const entities::EntityType m_type;
		std::unique_ptr<ColliderComponent> m_colliderComponent;
		std::unique_ptr<RenderComponent> m_renderComponent{};

		void Destroy();
	};
}
#endif