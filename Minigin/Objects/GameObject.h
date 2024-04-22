#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Minigin/Objects/Components/TransformComponent.h"
#include "TimeManager.h"

namespace dae
{
	class GameObject final
	{
	public:
		GameObject* GetParent() const;
		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		bool IsChild(GameObject* parent);
		int GetChildCount() const { return static_cast<int>(m_children.size()); }
		GameObject* GetChildAt(int index) const { return m_children[index]; }
		const std::vector<GameObject*>& GetChildren() const { return m_children; }

		template <typename ComponentType, typename... Args>
		ComponentType* AddComponent(const Args&... args)
		{
			std::unique_ptr<ComponentType> component{ std::make_unique<ComponentType>(this, args...) };
			ComponentType* ptr{ component.get() };
			m_components.push_back(std::move(component));

			return ptr;
		}

		template <typename ComponentType, typename Arg>
		ComponentType* AddComponent(Arg& arg)
		{
			std::unique_ptr<ComponentType> component{ std::make_unique<ComponentType>(this, arg) };
			ComponentType* ptr{ component.get() };
			m_components.push_back(std::move(component));

			return ptr;
		}

		template <typename ComponentType>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (ComponentType* castedComponent = dynamic_cast<ComponentType*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}

		bool HasComponent() const { return m_components.empty(); };

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y);
		void SetDead();

		TransformComponent* GetTransform() const { return m_transformComponent.get(); }
		bool IsDead() const { return m_isDead; }

		GameObject(float x = 0, float y = 0);
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		GameObject* m_parent{};
		std::vector<GameObject*> m_children{};
		dae::TimeManager& m_time = TimeManager::GetInstance();
		std::unique_ptr<TransformComponent> m_transformComponent;
		std::vector <std::unique_ptr<BaseComponent>> m_components{};

		bool m_isDead{ false };

		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);

		void RemoveComponent();
	};
}
#endif