#include <string>
#include <algorithm>
#include "GameObject.h"
#include "Components.h"
#include "Renderer.h"

dae::GameObject::GameObject(float x, float y)
	: m_transformComponent{ std::make_shared<TransformComponent>(this) }
{
	m_transformComponent = std::make_shared<TransformComponent>(this);
	m_transformComponent->SetPosition(x, y);
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
	m_components.push_back(std::move(component));
}

void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> component)
{
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void dae::GameObject::Update()
{
	std::for_each(m_components.begin(), m_components.end(), [&](std::shared_ptr<BaseComponent> component) {component->Update(); });
}

void dae::GameObject::LateUpdate()
{
	
}

void dae::GameObject::Render() const
{
	const auto& pos{ m_transformComponent->GetPosition() };

	for (auto component : m_components) component->Render(pos);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transformComponent->SetPosition(x, y);
}