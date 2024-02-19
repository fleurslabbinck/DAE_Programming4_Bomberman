#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class TextObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void AddGameObject(std::shared_ptr<GameObject> object);
		void RemoveGameObject(std::shared_ptr<GameObject> object);
		void RemoveAllGameObjects();

		void AddTextObject(std::shared_ptr<TextObject> object);
		void RemoveTextObject(std::shared_ptr<TextObject> object);
		void RemoveAllTextObjects();

		void FixedUpdate(float deltaTime);
		void Update(float deltaTime);
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_gameObjects{};
		std::vector < std::shared_ptr<TextObject>> m_textObjects{};

		static unsigned int m_idCounter; 
	};

}
