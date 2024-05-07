#include "CollisionManager.h"

#include "GameObject.h"

namespace dae
{
	void CollisionManager::RemoveCollider(ColliderComponent* collider)
	{
		std::erase(m_collidersComponents, collider);
	}

	void CollisionManager::FixedUpdate()
	{
		for (ColliderComponent* colliderComp : m_collidersComponents)
			if (colliderComp->GetObjectType() == ObjectType::Dynamic) colliderComp->Update();
	}

	bool CollisionManager::CanMove(const ColliderComponent* colliderComp, glm::vec2 nextPos) const
	{
		if (colliderComp == nullptr || colliderComp->GetObjectType() != ObjectType::Dynamic) return false;

		for (const ColliderComponent* otherColliderComp : m_collidersComponents)
		{
			if (otherColliderComp->GetObjectType() == ObjectType::Dynamic) continue;

			if (nextPos == otherColliderComp->GetCenter()) return false;
		}

		return true;
	}

	bool CollisionManager::AreColliding(const Collider& collider, const Collider& otherCollider) const
	{
		bool horizontalCollision{ collider.pos.x <= otherCollider.pos.x + collider.width && collider.pos.x + collider.width >= otherCollider.pos.x };
		bool verticalCollision{ collider.pos.y <= otherCollider.pos.y + collider.height && collider.pos.y + collider.height >= otherCollider.pos.y };

		if (horizontalCollision && verticalCollision)
		{
			int idk{};
			idk = 1;
		}

		return horizontalCollision && verticalCollision;
	}
}