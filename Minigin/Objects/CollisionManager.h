#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <glm/glm.hpp>
#include <vector>
#include "../Singleton.h"
#include "Components/ColliderComponent.h"

namespace dae
{
	using Event = int;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void SetCollisionEvent(Event event) { m_collisionEvent = event; }

		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);

		void FixedUpdate();

		bool CanMove(const ColliderComponent* colliderComp, glm::vec2 nextPos) const;

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
		~CollisionManager() = default;

		Event m_collisionEvent{};
		std::vector<ColliderComponent*> m_dynamicColliders{};
		std::vector<ColliderComponent*> m_staticColliders{};

		bool AreColliding(const Collider& collider, const Collider& otherCollider) const;
	};
}
#endif