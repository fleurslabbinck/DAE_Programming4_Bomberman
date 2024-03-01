#include "Scene.h"
#include "GameObject.h"
#include "TimeManager.h"

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

void Scene::Update()
{
	std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [&](std::shared_ptr<GameObject> go) 
		{ 
			if (!go->IsDead()) go->Update();
			else ++m_deadGameObjects;
	});
}

void Scene::LateUpdate()
{
	for (size_t idx{}; idx < m_gameObjects.size(); ++idx)
	{
		if (m_gameObjects[idx]->IsDead()) RemoveGameObject(m_gameObjects[idx]);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_gameObjects)
	{
		object->Render();
	}
}

