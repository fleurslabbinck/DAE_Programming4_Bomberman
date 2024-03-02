#pragma once
#include <memory>
#include <vector>
#include "Components.h"

namespace dae
{
	class Texture2D;
	class BaseComponent;

	class GameObject final
	{
	public:
		GameObject* GetParent() const { return m_parent; }
		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		void SetParentOnly(GameObject* parent) { m_parent = parent; };
		int GetChildCount() const { return m_childCount; }
		GameObject* GetChildAt(int index) const { return m_children[index]; }

		void AddComponent(std::shared_ptr<BaseComponent> component) { m_components.push_back(std::move(component)); };
		void RemoveComponent(std::shared_ptr<BaseComponent> component);
		template <typename C>
		std::shared_ptr<C> GetComponent() const
		{
			for (auto& component : m_components)
			{
				if (std::shared_ptr<C> castedComponent = std::dynamic_pointer_cast<C>(component)) return castedComponent;
			}

			return nullptr;
		};
		bool HasComponent() const { return m_components.empty(); };

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y, float z = 0);
		void SetDead() { m_isDead = true; }

		std::shared_ptr<TransformComponent> GetTransform() const { return m_transformComponent; }
		bool IsDead() const { return m_isDead; }

		GameObject(float x = 0, float y = 0, float z = 0);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		GameObject* m_parent{};
		std::vector<GameObject*> m_children{};
		int m_childCount{};

		std::shared_ptr<TransformComponent> m_transformComponent;

		std::vector <std::shared_ptr<BaseComponent>> m_components{};

		bool m_isDead{ false };

		bool IsChild(GameObject* parent);
		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);
	};
}
