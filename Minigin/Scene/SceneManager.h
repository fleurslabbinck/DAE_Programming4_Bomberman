#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "../Singleton.h"
#include "Scene.h"

namespace dae
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes;
	};
}
#endif