#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>

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
		m_scenes.push_back(std::unique_ptr<Scene>(new Scene{ name }));
		return *m_scenes.back();
	}

	void SceneManager::RemoveScene(const std::string& name)
	{
		auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const std::unique_ptr<Scene>& scene) { return scene.get()->GetName() == name; });
	}
}