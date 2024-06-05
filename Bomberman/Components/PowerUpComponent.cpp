#include "PowerUpComponent.h"

#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Objects/Components/RenderComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	//POWERUPCOMPONENT
	//---------------------------------
	PowerUpComponent::PowerUpComponent(GameObject* pOwner, entities::EntityType powerUpType)
		: BaseComponent(pOwner),
		m_type{ powerUpType },
		m_colliderComponent{ std::make_unique<ColliderComponent>(pOwner, glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL)) }
	{
		CollisionManager::GetInstance().AddCollider(m_colliderComponent.get());

		std::string filepath{};

		switch (m_type)
		{
		case entities::EntityType::Bombs:
			filepath = "Sprites/Bombs.png";
			break;
		case entities::EntityType::Fire:
			filepath = "Sprites/Fire.png";
			break;
		case entities::EntityType::Detonator:
			filepath = "Sprites/Detonator.png";
			break;
		}

		m_renderComponent = std::make_unique<RenderComponent>(pOwner, filepath);

		m_subComponents.push_back(m_colliderComponent.get());
		m_subComponents.push_back(m_renderComponent.get());
	}

	PowerUpComponent::~PowerUpComponent()
	{
		CollisionManager::GetInstance().RemoveCollider(m_colliderComponent.get());
	}

	void PowerUpComponent::OnNotify(Event event, GameObject* gameObject)
	{
		if (event == static_cast<int>(GameEvent::PLAYER_POWERUP))
		{
			GameEvent gameEvent{};

			switch (m_type)
			{
			case entities::EntityType::Bombs:
				gameEvent = GameEvent::BOMBS;
				break;
			case entities::EntityType::Fire:
				gameEvent = GameEvent::FIRE;
				break;
			case entities::EntityType::Detonator:
				gameEvent = GameEvent::DETONATOR;
				break;
			}

			Notify(static_cast<int>(gameEvent), gameObject);
			Destroy();
		}
	}

	void PowerUpComponent::Destroy()
	{
		GetOwner()->SetDead();
	}
}