#include "BombComponent.h"

#include "Objects/CollisionManager.h"
#include "Objects/Components/ColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	BombComponent::BombComponent(GameObject* pOwner, HealthComponent* bombermanHealthComp, uint8_t fire)
		: BaseComponent(pOwner), m_colliderComponent{ std::make_unique<ColliderComponent>(pOwner, glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) },
		m_healthComponent{ std::make_unique<HealthComponent>(pOwner, entities::EntityType::Explosion, 1) },
		m_spriteComponentBomb{ std::make_unique<SpriteComponent>(pOwner, "Sprites/Bomb.png", entities::EntityType::Bomb) },
		m_spriteComponentExplosion{ std::make_unique<SpriteComponent>(pOwner, "Sprites/Explosion.png", entities::EntityType::Explosion) },
		m_fire{ fire }
	{
		CollisionManager::GetInstance().AddCollider(m_colliderComponent.get());

		m_spriteComponentExplosion.get()->AddObserver(bombermanHealthComp);
		bombermanHealthComp->AddObserver(m_spriteComponentExplosion.get());

		m_subComponents.push_back(m_colliderComponent.get());
		m_subComponents.push_back(m_healthComponent.get());
		m_subComponents.push_back(m_spriteComponentBomb.get());
		m_subComponents.push_back(m_spriteComponentExplosion.get());
	}

	void BombComponent::Explode()
	{
		GetOwner()->SetDead();
		CollisionManager::GetInstance().RemoveCollider(m_colliderComponent.get());
	}
}