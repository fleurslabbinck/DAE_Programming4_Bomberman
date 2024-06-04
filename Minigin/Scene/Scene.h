#ifndef SCENE_H
#define SCENE_H

#include "../Objects/GameObject.h"

namespace dae
{
	class GameObject;
	class SceneManager;

	class Scene final
	{
		friend class SceneManager;
	public:
		GameObject* AddGameObject(std::unique_ptr<GameObject> object);
		void RemoveAllGameObjects();

		const std::string GetName() const { return m_name; }
		std::vector<GameObject*> GetGameObjects(const std::string& name);
		void PlaceOnTop(const std::string& name);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		void CleanUpDeadObjects();

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_gameObjects{};

		static unsigned int m_idCounter; 
	};

}
#endif