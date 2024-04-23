#include "TransformComponent.h"

#include "../GameObject.h"

namespace dae
{
	//---------------------------------
	//TRANSFORMCOMPONENT
	//---------------------------------
	TransformComponent::TransformComponent(GameObject* pOwner, float x, float y)
		: BaseComponent(pOwner), m_localPosition{ x, y }
	{

	}

	void TransformComponent::Translate(const glm::vec2& offset)
	{
		m_localPosition += offset;
		SetLocalPosition(m_localPosition);
	}

	void TransformComponent::UpdateWorldPosition()
	{
		if (GetOwner()->GetParent() == nullptr) m_worldPosition = m_localPosition;
		else m_worldPosition = GetOwner()->GetParent()->GetTransform()->GetWorldPosition() + m_localPosition;

		m_positionIsDirty = false;
	}

	void TransformComponent::SetLocalPosition(const glm::vec2& pos)
	{
		m_localPosition = pos;
		SetPositionDirty();
	}

	void TransformComponent::SetPositionDirty()
	{
		m_positionIsDirty = true;

		GameObject* owner{ GetOwner() };

		for (int idx{}; idx < owner->GetChildCount(); ++idx)
		{
			owner->GetChildAt(idx)->GetTransform()->SetPositionDirty();
		}

	}

	glm::vec2 TransformComponent::GetWorldPosition()
	{
		if (m_positionIsDirty) UpdateWorldPosition();

		return m_worldPosition;
	}
}