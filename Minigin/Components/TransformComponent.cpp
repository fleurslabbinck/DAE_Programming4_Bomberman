#include "GameObject.h"

//---------------------------------
//TRANSFORMCOMPONENT
//---------------------------------
void dae::TransformComponent::Translate(const glm::vec2& offset)
{
	m_localPosition += offset;
	SetLocalPosition(m_localPosition);
}

void dae::TransformComponent::UpdateWorldPosition()
{
	if (GetOwner()->GetParent() == nullptr) m_worldPosition = m_localPosition;
	else m_worldPosition = GetOwner()->GetParent()->GetTransform()->GetWorldPosition() + m_localPosition;

	m_positionIsDirty = false;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

void dae::TransformComponent::SetPositionDirty()
{
	m_positionIsDirty = true;

	GameObject* owner{ GetOwner() };

	for (int idx{}; idx < owner->GetChildCount(); ++idx)
	{
		owner->GetChildAt(idx)->GetTransform()->SetPositionDirty();
	}

}

glm::vec2 dae::TransformComponent::GetWorldPosition()
{
	if (m_positionIsDirty) UpdateWorldPosition();

	return m_worldPosition;
}
