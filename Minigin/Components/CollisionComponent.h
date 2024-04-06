#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//COLLISIONCOMPONENT
	//---------------------------------
	class CollisionComponent final : public BaseComponent, public Subject
	{
	public:
		enum class EntityType {
			Player,
			Enemy,
			Block,
			Bomb,
		};

		struct Sprite {
			float width, height;
			glm::vec2 center{};
		};

		glm::vec2 GetCenter() const { return m_collider.center; }
		bool HandleCollision(const glm::vec2 pos, const std::vector<GameObject*>& entities) const;
		bool CanMove(const std::vector<GameObject*>& entities) const;
		Sprite GetCollider() const { return m_collider; }
		EntityType GetEntity() const { return m_entityType; }

		explicit CollisionComponent(GameObject* pOwner, EntityType entityType, glm::vec2 dimensions);
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;

	private:
		EntityType m_entityType;
		Sprite m_collider;

		bool IsColliding(const glm::vec2 pos, const glm::vec2 entityPos, const Sprite& collider) const;
	};
}
#endif