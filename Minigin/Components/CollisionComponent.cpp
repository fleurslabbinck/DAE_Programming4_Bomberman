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
				const EntityType type{ collisionComp->GetEntity() };

				if (type == EntityType::Block) continue;

				const glm::vec2 entityPos{ entity->GetTransform()->GetLocalPosition() };
				const Sprite entityCollider{ collisionComp->GetCollider() };

				if (IsColliding(pos, entityPos, entityCollider))
				{
					switch (m_entityType)
					{
					case dae::CollisionComponent::EntityType::Player:

						if (type == EntityType::Enemy || type == EntityType::Bomb)
						{
							Notify(GameEvent::PLAYER_DIED, GetOwner());
							return true;
						}

						break;

					case dae::CollisionComponent::EntityType::Enemy:

						if (type == EntityType::Player)
						{
							Notify(GameEvent::PLAYER_DIED, entity);
							return true;
						}

						if (type == EntityType::Bomb)
						{
							Notify(GameEvent::ENEMY_KILLED, GetOwner());
							return true;
						}

						break;

					case dae::CollisionComponent::EntityType::Bomb:

						if (type == EntityType::Enemy)
						{
							Notify(GameEvent::ENEMY_KILLED, entity);
							return true;
						}

						if (type == EntityType::Player)
						{
							Notify(GameEvent::PLAYER_DIED, entity);
							return true;
						}

						if (type == EntityType::Wall)
						{
							Notify(GameEvent::WALL_DESTROYED, entity);
							return true;
						}

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
			if (const CollisionComponent* collisionComp = entity->GetComponent<CollisionComponent>())
				if (collisionComp->GetEntity() == EntityType::Block || collisionComp->GetEntity() == EntityType::Wall) return false;
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