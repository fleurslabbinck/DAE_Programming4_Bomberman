#include <string>
#include <algorithm>
#include "GameObject.h"
#include "Components.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_components.emplace_back(std::move(component));
}

void dae::GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void dae::GameObject::FixedUpdate(float deltaTime)
{
	std::for_each(m_components.begin(), m_components.end(), [=](std::shared_ptr<Component> component) {component.get()->FixedUpdate(deltaTime); });
}

void dae::GameObject::Update(float deltaTime)
{
	std::for_each(m_components.begin(), m_components.end(), [=](std::shared_ptr<Component> component) {component.get()->Update(deltaTime); });
}

void dae::GameObject::Render() const
{
	for (auto component : m_components) component.get()->Render(m_transform);

	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
