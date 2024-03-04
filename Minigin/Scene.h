#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void AddGameObject(std::unique_ptr<GameObject> object);
		void RemoveGameObject(std::unique_ptr<GameObject> object);
		void RemoveAllGameObjects();

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_gameObjects{};

		//float m_accumulatedTime{};
		//bool m_deleted{ false };

		static unsigned int m_idCounter; 
	};

}
