#include "ExitComponent.h"

#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Objects/Components/RenderComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	//EXITCOMPONENT
	//---------------------------------
	ExitComponent::ExitComponent(GameObject* pOwner, uint8_t totalEnemies)
		: BaseComponent(pOwner),
		m_totalEnemies{ totalEnemies },
		m_colliderComponent{ std::make_unique<ColliderComponent>(pOwner, glm::vec2{ 1.f, 0 }, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL)) },
		m_renderComponent{ std::make_unique<RenderComponent>(pOwner, "Sprites/Exit.png") }
	{
		CollisionManager::GetInstance().AddCollider(m_colliderComponent.get());

		m_subComponents.push_back(m_colliderComponent.get());
		m_subComponents.push_back(m_renderComponent.get());
	}

	ExitComponent::~ExitComponent()
	{
		CollisionManager::GetInstance().RemoveCollider(m_colliderComponent.get());
	}

	void ExitComponent::OnNotify(Event event, GameObject* gameObject)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::ENEMY_DEATH):
			++m_killedEnemies;
			break;
		case static_cast<int>(GameEvent::PLAYER_EXIT):
			if (m_killedEnemies >= m_totalEnemies)
			{
				Notify(static_cast<int>(GameEvent::STAGE_WON), gameObject);
			}
			break;
		}
	}
}