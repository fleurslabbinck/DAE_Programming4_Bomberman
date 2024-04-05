#include <algorithm>
#include "GameObject.h"

namespace dae
{
	GameObject::GameObject(float x, float y)
		: m_transformComponent{ std::make_unique<TransformComponent>(this, x, y) }
	{
	}

	GameObject::~GameObject()
	{
		m_transformComponent.reset();

		for (std::unique_ptr<BaseComponent>& component : m_components) component.reset();
	}

	void GameObject::RemoveComponent()
	{
		const auto it = std::stable_partition(m_components.begin(), m_components.end(), [](const std::unique_ptr<BaseComponent>& component) { return !component->ShouldBeDeleted(); });

		m_components.erase(it, m_components.end());
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
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

	bool GameObject::IsChild(GameObject* parent)
	{
		//return std::any_of(m_children.begin(), m_children.end(), [&](GameObject* child) { return parent != child; });
		return std::any_of(m_children.begin(), m_children.end(), [&](GameObject* child) { return child->IsChild(parent); });
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		//ckeck if valid
		if (child == nullptr || child->IsChild(child)) return;

		//remove given child from children
		if (!m_children.empty())
		{
			m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
		}
	}

	void GameObject::AddChild(GameObject* child)
	{
		//check if valid
		if (child == nullptr || child == m_parent) return;

		//add child to children list
		m_children.push_back(child);
	}

	void GameObject::FixedUpdate()
	{
		for (std::unique_ptr<BaseComponent>& component : m_components) if (!component->ShouldBeDeleted()) component->FixedUpdate();
	}

	void GameObject::Update()
	{
		for (std::unique_ptr<BaseComponent>& component : m_components) if (!component->ShouldBeDeleted()) component->Update();
	}

	void GameObject::LateUpdate()
	{
		for (std::unique_ptr<BaseComponent>& component : m_components) if (!component->ShouldBeDeleted()) component->LateUpdate();

		RemoveComponent();
	}

	void GameObject::Render() const
	{
		if (!m_transformComponent) return;

		const auto& pos{ m_transformComponent->GetWorldPosition() };

		for (const std::unique_ptr<BaseComponent>& component : m_components) if (!component->ShouldBeDeleted()) component->Render(pos);
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transformComponent->SetLocalPosition(glm::vec2(x, y));
	}

	void GameObject::SetDead()
	{
		m_isDead = true;
		SetParent(nullptr);

		for (GameObject* child : m_children)
		{
			child->SetDead();
		}

		for (std::unique_ptr<BaseComponent>& component : m_components) component.get()->SetDelete();
	}
}