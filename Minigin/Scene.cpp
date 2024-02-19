#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_gameObjects.emplace_back(std::move(gameObject));
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject), m_gameObjects.end());
}

void Scene::RemoveAllGameObjects()
{
	m_gameObjects.clear();
}

void Scene::FixedUpdate(float deltaTime)
{
	for (auto& object : m_gameObjects)
	{
		object->FixedUpdate(deltaTime);
	}
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_gameObjects)
	{
		object->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_gameObjects)
	{
		object->Render();
	}
}

