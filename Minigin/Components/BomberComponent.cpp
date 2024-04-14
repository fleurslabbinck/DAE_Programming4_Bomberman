#include "BomberComponent.h"

#include "GameObject.h"
#include "Components/CollisionComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/HealthComponent.h"

namespace dae
{
	BomberComponent::BomberComponent(GameObject* pOwner, Scene& scene)
		: BaseComponent(pOwner), m_scene{ scene }
	{

	}

	void BomberComponent::DropBomb(GameObject* parent, HealthComponent* healthComp, const glm::vec2& pos)
	{
		GameObject* bomb{ m_scene.AddGameObject(std::make_unique<GameObject>(pos.x, pos.y)) };
		CollisionComponent* collisionComp{ bomb->AddComponent<CollisionComponent>(CollisionComponent::EntityType::Wall) };
		SpriteComponent* spriteComp{ bomb->AddComponent<SpriteComponent>("Sprites/Bomb.png", SpriteComponent::SpriteType::BOMB)};
		// ExplosionComponent
		spriteComp->AddObserver(collisionComp);
		healthComp->AddObserver(spriteComp);

		bomb->SetParent(parent);
	}
}