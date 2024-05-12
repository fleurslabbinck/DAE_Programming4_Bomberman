#include "Scene.h"
#include <algorithm>
#include "../Objects/CollisionManager.h"

namespace dae
{
	unsigned int Scene::m_idCounter = 0;

	Scene::Scene(const std::string& name) : m_name(name)
	{
	
	}

	GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
	{
		m_gameObjects.emplace_back(std::move(gameObject));
		return m_gameObjects.back().get();
	}

	void Scene::CleanUpDeadObjects()
	{
		const auto it = std::stable_partition(m_gameObjects.begin(), m_gameObjects.end(), [](const std::unique_ptr<GameObject>& gameObject) { return !gameObject->IsDead(); });

		m_gameObjects.erase(it, m_gameObjects.end());
	}

	void Scene::RemoveAllGameObjects()
	{
		for (auto& gameObject : m_gameObjects) gameObject->SetDead();
	}

	void Scene::FixedUpdate()
	{
		CollisionManager::GetInstance().FixedUpdate();
	}

	void Scene::Update()
	{
		for (std::unique_ptr<GameObject>& gameObject : m_gameObjects)
		{
			if (!gameObject.get()->IsDead()) gameObject.get()->Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (std::unique_ptr<GameObject>& gameObject : m_gameObjects)
		{
			if (!gameObject.get()->IsDead()) gameObject.get()->LateUpdate();
		}

		CleanUpDeadObjects();
	}

	void Scene::Render() const
	{
		for (const std::unique_ptr<GameObject>& gameObject : m_gameObjects)
		{
			gameObject.get()->Render();
		}
	}
}