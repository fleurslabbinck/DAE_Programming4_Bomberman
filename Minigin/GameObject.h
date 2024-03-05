#pragma once
#include <memory>
#include <vector>
#include "Components.h"

namespace dae
{
	class GameObject final
	{
	public:
		GameObject* GetParent() const { return m_parent; }
		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		bool IsChild(GameObject* parent);
		int GetChildCount() const { return static_cast<int>(m_children.size()); }
		GameObject* GetChildAt(int index) const { return m_children[index]; }

		void AddComponent(std::unique_ptr<BaseComponent> component) { m_components.push_back(std::move(component)); };
		template <typename C>
		C* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (C* castedComponent = dynamic_cast<C*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		};
		bool HasComponent() const { return m_components.empty(); };

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y, float z = 0);
		void SetDead();

		TransformComponent* GetTransform() const { return m_transformComponent.get(); }
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

		std::unique_ptr<TransformComponent> m_transformComponent;

		std::vector <std::unique_ptr<BaseComponent>> m_components{};

		bool m_isDead{ false };

		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);

		void RemoveComponent();
	};
}
