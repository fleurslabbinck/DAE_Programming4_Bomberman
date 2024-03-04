#include <string>
#include <algorithm>
#include "GameObject.h"
#include "Components.h"
#include "Renderer.h"
#include "TimeManager.h"

dae::GameObject::GameObject(float x, float y, float z)
	: m_transformComponent{ std::make_shared<TransformComponent>(this) }
{
	m_transformComponent->SetLocalPosition(glm::vec3(x, y, z));
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> component)
{
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	//check if valid
	if (IsChild(parent) || parent == this || m_parent == parent) return;

	if (parent == nullptr) m_transformComponent->SetLocalPosition(m_transformComponent->GetWorldPosition());
	else
	{
 		if (keepWorldPosition) m_transformComponent->SetLocalPosition(m_transformComponent->GetLocalPosition() - parent->GetTransform()->GetWorldPosition());

		m_transformComponent->SetPositionDirty();
	}

	//remove from previous parent
	if (m_parent != nullptr) m_parent->RemoveChild(this);

	//set parent on itself
	m_parent = std::move(parent);

	//add itself as child
	if (m_parent != nullptr) m_parent->AddChild(this);
}

bool dae::GameObject::IsChild(GameObject* parent)
{
	//return std::any_of(m_children.begin(), m_children.end(), [&](GameObject* child) { return parent != child; });
	return std::any_of(m_children.begin(), m_children.end(), [&](GameObject* child) { return child->IsChild(parent); });
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	//ckeck if valid
	if (child == nullptr || child->IsChild(child)) return;

	//remove given child from children
	if (!m_children.empty())
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}

	//remove itself as parent of child
	//if (child->GetParent() != nullptr) child->SetParent(nullptr);
	//child->SetParentOnly(nullptr);
}

void dae::GameObject::AddChild(GameObject* child)
{
	//check if valid
	if (child == nullptr || child == m_parent) return;

	//remove given child from child's previous parent
	//if (!child->GetParent()->m_children.empty()) child->GetParent()->RemoveChild(child);

	//set itself as parent
	//child->SetParent(this);

	//add child to children list
	m_children.push_back(child);
}

void dae::GameObject::FixedUpdate()
{
	std::for_each(m_components.begin(), m_components.end(), [&](std::shared_ptr<BaseComponent> component) { component->FixedUpdate(); });
}

void dae::GameObject::Update()
{
	std::for_each(m_components.begin(), m_components.end(), [&](std::shared_ptr<BaseComponent> component)
		{
			if (!component->ShouldBeDeleted()) component->Update();
		}
	);
}

void dae::GameObject::LateUpdate()
{
	for (size_t idx{}; idx < m_components.size(); ++idx)
	{
		if (m_components[idx]->ShouldBeDeleted()) RemoveComponent(m_components[idx]);
	}
}

void dae::GameObject::Render() const
{
	const auto& pos{ m_transformComponent->GetWorldPosition() };

	for (auto component : m_components) component->Render(pos);
}

void dae::GameObject::SetPosition(float x, float y, float z)
{
	m_transformComponent->SetLocalPosition(glm::vec3(x, y, z));
}