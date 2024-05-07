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
}