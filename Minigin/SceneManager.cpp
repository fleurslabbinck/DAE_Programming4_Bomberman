#include "SceneManager.h"
#include "Scene.h"

namespace dae
{
	void SceneManager::FixedUpdate()
	{
		for (auto& scene : m_scenes)
		{
			scene->FixedUpdate();
		}
	}

	void SceneManager::Update()
	{
		for (auto& scene : m_scenes)
		{
			scene->Update();
		}
	}

	void SceneManager::LateUpdate()
	{
		for (auto& scene : m_scenes)
		{
			scene->LateUpdate();
		}
	}

	void SceneManager::Render()
	{
		for (const auto& scene : m_scenes)
		{
			scene->Render();
		}
	}

	Scene& SceneManager::CreateScene(const std::string& name)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene(name));
		m_scenes.push_back(scene);
		return *scene;
	}
}