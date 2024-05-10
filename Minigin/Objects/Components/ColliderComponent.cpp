#include "ColliderComponent.h"

#include "../GameObject.h"

namespace dae
{
	ColliderComponent::ColliderComponent(GameObject* pOwner, glm::vec2 offset, float width, float height, bool isDynamic)
		: BaseComponent(pOwner)
		, m_offset{ offset }
	{
		UpdatePos();

		m_collider.width = width;
		m_collider.height = height;

		if (!isDynamic) m_objectType = ObjectType::Static;
	}

	void  ColliderComponent::UpdatePos()
	{
		m_collider.pos = GetOwner()->GetTransform()->GetWorldPosition() + m_offset;
	}

	glm::vec2 ColliderComponent::GetLocalCenter() const
	{
		const glm::vec2 loalPos{ GetOwner()->GetTransform()->GetLocalPosition() };

		return { loalPos.x + m_collider.width / 2, loalPos.y + m_collider.height / 2 };
	}
}