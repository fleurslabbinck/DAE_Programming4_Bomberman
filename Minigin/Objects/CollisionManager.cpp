#include "CollisionManager.h"

#include "GameObject.h"

namespace dae
{
	void CollisionManager::AddCollider(ColliderComponent* collider)
	{
		switch (collider->GetObjectType())
		{
		case ObjectType::Dynamic:
			m_dynamicColliders.push_back(collider);
			break;
		case ObjectType::Static:
			m_staticColliders.push_back(collider);
		}
	}

	void CollisionManager::RemoveCollider(ColliderComponent* collider)
	{
		switch (collider->GetObjectType())
		{
		case ObjectType::Dynamic:
			std::erase(m_dynamicColliders, collider);
			break;
		case ObjectType::Static:
			std::erase(m_staticColliders, collider);
		}
	}

	void CollisionManager::FixedUpdate()
	{
		for (ColliderComponent* collider : m_dynamicColliders)
		{
			collider->UpdatePos();

			for (ColliderComponent* otherCollider : m_dynamicColliders)
			{
				if (collider == otherCollider) continue;

				if (AreColliding(collider->GetCollider(), otherCollider->GetCollider())) collider->OnCollision(m_collisionEvent, otherCollider->GetOwningGameObject());
			}
		}
	}

	bool CollisionManager::CanMove(const ColliderComponent* colliderComp, glm::vec2 nextPos) const
	{
		if (colliderComp == nullptr || colliderComp->GetObjectType() != ObjectType::Dynamic) return false;

		for (const ColliderComponent* otherColliderComp : m_staticColliders)
		{
			if (nextPos == otherColliderComp->GetLocalCenter()) return false;
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