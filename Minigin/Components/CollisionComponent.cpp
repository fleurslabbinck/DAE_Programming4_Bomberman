#include "CollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GridComponent.h"

//---------------------------------
//COLLISIONCOMPONENT
//---------------------------------
namespace dae
{
	CollisionComponent::CollisionComponent(GameObject* pOwner, EntityType entityType, glm::vec2 dimensions)
		: BaseComponent(pOwner), m_entityType{ entityType }, m_collider{ dimensions.x, dimensions.y, {dimensions.x / 2.f, dimensions.y / 2.f } }
	{

	}

	bool CollisionComponent::HandleCollision(const glm::vec2 pos, const std::vector<GameObject*>& entities) const
	{
		for (const GameObject* entity : entities)
		{
			if (const CollisionComponent* collisionComp = entity->GetComponent<CollisionComponent>())
			{
				if (collisionComp->GetEntity() == EntityType::Block) continue;

				const glm::vec2 entityPos{ entity->GetTransform()->GetWorldPosition() };
				const Sprite entityCollider{ collisionComp->GetCollider() };

				if (IsColliding(pos, entityPos, entityCollider))
				{
					switch (collisionComp->GetEntity())
					{
					case dae::CollisionComponent::EntityType::Player:
						if (m_entityType == EntityType::Enemy)
						{
							Notify(GameEvent::HEALTH_CHANGED, entity);
							return true;
						}
						break;
					case dae::CollisionComponent::EntityType::Enemy:
						if (m_entityType == EntityType::Player)
						{
							Notify(GameEvent::HEALTH_CHANGED, GetOwner());
							return true;
						}
						break;
					case dae::CollisionComponent::EntityType::Bomb:
						Notify(GameEvent::HEALTH_CHANGED, GetOwner());
						return true;
						break;
					}
				}
			}
		}

		return false;
	}

	bool CollisionComponent::CanMove(const std::vector<GameObject*>& entities) const
	{
		for (const GameObject* entity : entities)
		{
			if (const CollisionComponent* collisionComp = entity->GetComponent<CollisionComponent>()) if (collisionComp->GetEntity() == EntityType::Block) return false;
		}

		return true;
	}

	bool CollisionComponent::IsColliding(const glm::vec2 pos, const glm::vec2 entityPos, const Sprite& collider) const
	{
		constexpr float collisionOffset{ 0.2f };

		bool horizontalCollision = pos.x + collisionOffset <= entityPos.x + collider.width && pos.x - collisionOffset + m_collider.width >= entityPos.x;
		bool verticalCollision = pos.y + collisionOffset <= entityPos.y + collider.height && pos.y - collisionOffset + m_collider.height >= entityPos.y;

		return horizontalCollision && verticalCollision;
	}
}