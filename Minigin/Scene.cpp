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

void Scene::CleanUpDeadObjects()
{
	const auto it = std::stable_partition(m_gameObjects.begin(), m_gameObjects.end(), [](const std::unique_ptr<GameObject>& gameObject) { return !gameObject->IsDead(); });

	m_gameObjects.erase(it, m_gameObjects.end());
}

void Scene::RemoveAllGameObjects()
{
	m_gameObjects.clear();
}

void Scene::FixedUpdate()
{
	for (std::unique_ptr<GameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject.get()->IsDead()) gameObject.get()->FixedUpdate();
	}
}

void Scene::Update()
{
	//tests
	//constexpr float maxTime{ 5.f };
	//dae::TimeManager& time = TimeManager::GetInstance();
	//
	//m_accumulatedTime += time.GetDeltaTime();
	//
	//if (m_accumulatedTime >= maxTime && !m_deleted)
	//{
	//	//m_gameObjects[6]->SetParent(nullptr);
	//	m_deleted = true;
	//	//m_gameObjects[3].get()->GetComponent<FPSComponent>()->SetDelete();
	//	//m_gameObjects[7].get()->SetDead();
	//}

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

