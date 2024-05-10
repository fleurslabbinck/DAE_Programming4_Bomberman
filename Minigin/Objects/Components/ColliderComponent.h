#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "BaseComponent.h"
#include "../../Observers/Subject.h"

namespace dae
{
	class GameObject;

	enum class ObjectType {
		Dynamic,
		Static,
	};

	struct Collider {
		glm::vec2 pos{};
		float width{};
		float height{};
	};

	//---------------------------------
	//COLLISIONCOMPONENT
	//---------------------------------
	class ColliderComponent final : public BaseComponent, public Subject
	{
	public:
		void UpdatePos();

		ObjectType GetObjectType() const { return m_objectType; }
		Collider GetCollider() const { return m_collider; }
		glm::vec2 GetCenter() const { return { m_collider.pos.x + m_collider.width / 2, m_collider.pos.y + m_collider.height / 2 }; }
		glm::vec2 GetLocalCenter() const;

		explicit ColliderComponent(GameObject* pOwner, glm::vec2 offset, float width, float height, bool isDynamic = true);
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;

	private:
		ObjectType m_objectType{ ObjectType::Dynamic };
		Collider m_collider{};
		const glm::vec2 m_offset;
	};
}
#endif