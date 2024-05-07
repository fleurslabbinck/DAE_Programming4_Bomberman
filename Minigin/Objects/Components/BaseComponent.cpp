#include "BaseComponent.h"

#include "../GameObject.h"

namespace dae
{
	//---------------------------------
	//BASE COMPONENT
	//---------------------------------
	void BaseComponent::Update() { for (auto& component : m_subComponents) component->Update(); }

	void BaseComponent::LateUpdate() { for (auto& component : m_subComponents) component->LateUpdate(); }

	void BaseComponent::Render(const glm::vec2& pos) const { for (auto& component : m_subComponents) component->Render(pos); }
}