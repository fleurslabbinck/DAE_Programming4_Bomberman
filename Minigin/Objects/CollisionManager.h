#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <glm/glm.hpp>
#include <vector>
#include "../Singleton.h"
#include "Components/ColliderComponent.h"

namespace dae
{
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void AddCollider(ColliderComponent* collider) { if (collider) m_collidersComponents.push_back(collider); }
		void RemoveCollider(ColliderComponent* collider);

		void FixedUpdate();

		bool CanMove(const ColliderComponent* colliderComp, glm::vec2 nextPos) const;

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
		~CollisionManager() = default;

		std::vector<ColliderComponent*> m_collidersComponents{};

		bool AreColliding(const Collider& collider, const Collider& otherCollider) const;
	};
}
#endif