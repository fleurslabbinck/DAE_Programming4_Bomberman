#include "Components/BaseComponent.h"
#include "GameObject.h"

//---------------------------------
//BASE COMPONENT
//---------------------------------
void dae::BaseComponent::FixedUpdate() { for (auto& component : m_subComponents) component->FixedUpdate(); }

void dae::BaseComponent::Update() { for (auto& component : m_subComponents) component->Update(); }

void dae::BaseComponent::LateUpdate() { for (auto& component : m_subComponents) component->LateUpdate(); }

void dae::BaseComponent::Render(const glm::vec2& pos) const { for (auto& component : m_subComponents) component->Render(pos); }