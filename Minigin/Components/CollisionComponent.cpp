#include "CollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GridComponent.h"

//---------------------------------
//COLLISIONCOMPONENT
//---------------------------------
namespace dae
{
	CollisionComponent::CollisionComponent(GameObject* pOwner, EntityType entityType, float offset, const glm::vec2& dimensions)
		: BaseComponent(pOwner), m_entityType{ entityType }, m_collider{ offset, dimensions.x, dimensions.y, { (dimensions.x / 2.f) + offset, dimensions.y / 2.f } }
	{

	}

	void CollisionComponent::OnNotify(GameEvent event, GameObject* gameObject)
	{
		switch (event)
		{
		case dae::GameEvent::PLAYER_RESPAWN:
			gameObject->GetComponent<CollisionComponent>()->SetCheckForCollision(true);
			break;
		}
	}

	bool CollisionComponent::HandleCollision(const glm::vec2 pos, const std::vector<GameObject*>& entities) const
	{
		for (GameObject* entity : entities)
		{
			if (const CollisionComponent* collisionComp = entity->GetComponent<CollisionComponent>())
			{
				const EntityType type{ collisionComp->GetEntity() };

				if (type == EntityType::Block) continue;
				if (!entity->GetComponent<CollisionComponent>()->GetCheckForCollision()) continue;

				const glm::vec2 entityPos{ entity->GetTransform()->GetLocalPosition() };
				const Sprite entityCollider{ collisionComp->GetCollider() };

				if (IsColliding(pos, entityPos, entityCollider))
				{
					switch (type)
					{
					case dae::CollisionComponent::EntityType::Player:

						if (m_entityType != EntityType::Player)
						{
							HandleEntityHit(entity, type);
							return true;
						}

						break;

					case dae::CollisionComponent::EntityType::Enemy:

						if (m_entityType == EntityType::Player)
						{
							HandleEntityHit(GetOwner(), m_entityType);
							return true;
						}

						if (m_entityType == EntityType::Bomb)
						{
							HandleEntityHit(entity, type);
							return true;
						}

						break;

					case dae::CollisionComponent::EntityType::Wall:

						if (m_entityType == EntityType::Bomb)
						{
							HandleEntityHit(entity, type);
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
		constexpr float verticalOffset{ 5.f };

		bool horizontalCollision = pos.x + m_collider.offset <= entityPos.x + collider.width + collider.offset && pos.x + m_collider.offset + m_collider.width >= entityPos.x + collider.offset;
		bool verticalCollision = pos.y + verticalOffset <= entityPos.y + collider.height - verticalOffset && pos.y + m_collider.height - verticalOffset >= entityPos.y + verticalOffset;

		return horizontalCollision && verticalCollision;
	}

	void CollisionComponent::HandleEntityHit(GameObject* entity, EntityType type) const
	{
		switch (type)
		{
		case dae::CollisionComponent::EntityType::Player:
			Notify(GameEvent::PLAYER_HIT, entity);
			break;
		case dae::CollisionComponent::EntityType::Enemy:
			Notify(GameEvent::ENEMY_HIT, entity);
			break;
		case dae::CollisionComponent::EntityType::Wall:
			Notify(GameEvent::WALL_HIT, entity);
			break;
		default:
			break;
		}

		entity->GetComponent<CollisionComponent>()->SetCheckForCollision(false);
	}
}