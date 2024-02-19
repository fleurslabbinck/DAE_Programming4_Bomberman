#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h"

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

void Scene::AddTextObject(std::shared_ptr<TextObject> textObject)
{
	m_textObjects.emplace_back(std::move(textObject));
}

void Scene::RemoveTextObject(std::shared_ptr<TextObject> textObject)
{
	m_textObjects.erase(std::remove(m_textObjects.begin(), m_textObjects.end(), textObject), m_textObjects.end());
}

void Scene::RemoveAllTextObjects()
{
	m_textObjects.clear();
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

	for (auto& object : m_textObjects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_gameObjects)
	{
		object->Render();
	}

	for (auto& object : m_textObjects)
	{
		object->Render();
	}
}

