#include "BomberComponent.h"

#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Objects/Components/ColliderComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"

namespace dae
{
	BomberComponent::BomberComponent(GameObject* pOwner, Scene& scene)
		: BaseComponent(pOwner), m_scene{ scene }
	{

	}

	void BomberComponent::DropBomb(GameObject* parent, HealthComponent* healthComp, const glm::vec2& pos)
	{
		GameObject* bomb{ m_scene.AddGameObject(std::make_unique<GameObject>(pos.x, pos.y)) };
		ColliderComponent* collider{ bomb->AddComponent<ColliderComponent>(glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) };
		CollisionManager::GetInstance().AddCollider(collider);

		SpriteComponent* spriteComp{ bomb->AddComponent<SpriteComponent>("Sprites/Bomb.png", entities::EntityType::Bomb)};
		// ExplosionComponent
		spriteComp->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);

		bomb->SetParent(parent);
	}
}