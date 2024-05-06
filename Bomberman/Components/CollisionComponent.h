#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../Bomberman/BombermanUtil.h"

#include "../Minigin/Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	using Event = int;

	class GameObject;

	//---------------------------------
	//COLLISIONCOMPONENT
	//---------------------------------
	class CollisionComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		enum class EntityType {
			Player,
			Enemy,
			Block,
			Bomb,
			Wall,
		};

		struct Sprite {
			const float offset, width, height;
			glm::vec2 center{};
		};

		void OnNotify(Event event, GameObject* gameObject) override;
		void SetCheckForCollision(bool checkForCollision) { m_checkForCollision = checkForCollision; }
		bool GetCheckForCollision() const { return m_checkForCollision; }

		glm::vec2 GetCenter() const { return m_collider.center; }
		bool HandleCollision(const glm::vec2 pos, const std::vector<GameObject*>& entities) const;
		bool CanMove(const std::vector<GameObject*>& entities) const;
		Sprite GetCollider() const { return m_collider; }
		EntityType GetEntity() const { return m_entityType; }

		explicit CollisionComponent(GameObject* pOwner, EntityType entityType, float offset = 0, const glm::vec2& dimensions = { constants::GRIDCELL, constants::GRIDCELL });
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;

	private:
		EntityType m_entityType;
		Sprite m_collider;

		bool m_checkForCollision{ true };

		bool IsColliding(const glm::vec2 pos, const glm::vec2 entityPos, const Sprite& collider) const;
		void HandleEntityHit(GameObject* entity, EntityType type) const;
	};
}
#endif