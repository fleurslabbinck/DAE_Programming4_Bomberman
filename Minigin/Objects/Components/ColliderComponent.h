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

		void SetDelete() override;

		ObjectType GetObjectType() const { return m_objectType; }
		Collider GetCollider() const { return m_collider; }
		glm::vec2 GetLocalCenter() const;
		GameObject* GetOwningGameObject() const { return GetOwner(); }

		void OnCollision(Event event, GameObject* gameObject) const { Notify(event, gameObject); };

		explicit ColliderComponent(GameObject* pOwner, glm::vec2 offset, float width, float height, bool isDynamic = true, bool updatePos = true);
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;

	private:
		bool m_updatePos{ false };
		ObjectType m_objectType{ ObjectType::Dynamic };
		Collider m_collider{};
		const glm::vec2 m_offset;
	};
}
#endif