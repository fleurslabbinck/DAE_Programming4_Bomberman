#include "ColliderComponent.h"

#include "../GameObject.h"
#include "../CollisionManager.h"

namespace dae
{
	ColliderComponent::ColliderComponent(GameObject* pOwner, glm::vec2 offset, float width, float height, bool isDynamic, bool updatePos)
		: BaseComponent(pOwner), m_offset{ offset }, m_updatePos{ updatePos }
	{
		UpdatePos();

		m_collider.width = width;
		m_collider.height = height;

		if (!isDynamic) m_objectType = ObjectType::Static;
	}

	void  ColliderComponent::UpdatePos()
	{
		if (m_updatePos) m_collider.pos = GetOwner()->GetTransform()->GetWorldPosition() + m_offset;
	}

	void ColliderComponent::SetDelete()
	{
		m_delete = true;

		CollisionManager::GetInstance().RemoveCollider(this);
	}

	glm::vec2 ColliderComponent::GetLocalCenter() const
	{
		const glm::vec2 localPos{ GetOwner()->GetTransform()->GetLocalPosition() };

		return { localPos.x + m_collider.width / 2 + m_offset.x, localPos.y + m_collider.height / 2 + m_offset.y };
	}
}