#include "Scene.h"
#include "TimeManager.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	m_gameObjects.emplace_back(std::move(gameObject));
}

void Scene::RemoveGameObject(std::unique_ptr<GameObject> gameObject)
{
	m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject), m_gameObjects.end());
}

void Scene::RemoveAllGameObjects()
{
	m_gameObjects.clear();
}

void Scene::FixedUpdate()
{
	std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [](std::unique_ptr<GameObject>& go)
		{
			if (!go->IsDead()) go->FixedUpdate();
		}
	);
}

void Scene::Update()
{
	constexpr float maxTime{ 5.f };
	dae::TimeManager& time = TimeManager::GetInstance();
	
	m_accumulatedTime += time.GetDeltaTime();
	
	if (m_accumulatedTime >= maxTime && !m_deleted)
	{
		m_gameObjects[6]->SetParent(nullptr);
		m_deleted = true;
		m_gameObjects[7].get()->SetDead();
	}

	std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [](std::unique_ptr<GameObject>& go) 
		{ 
			if (!go->IsDead()) go->Update();
		}
	);
}

void Scene::LateUpdate()
{
	for (size_t idx{}; idx < m_gameObjects.size(); ++idx)
	{
		if (m_gameObjects[idx]->IsDead()) RemoveGameObject(std::move(m_gameObjects[idx]));
	}
}

void Scene::Render() const
{
	for (const auto& object : m_gameObjects)
	{
		object->Render();
	}
}

