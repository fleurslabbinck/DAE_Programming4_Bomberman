#pragma once
#include <memory>
#include <vector>
#include "Components.h"
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class BaseComponent;

	class GameObject final
	{
	public:
		void AddComponent(std::shared_ptr<BaseComponent> object);
		void RemoveComponent(std::shared_ptr<BaseComponent> object);
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

		void Update();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y);
		void SetDead() { m_isDead = true; }

		std::shared_ptr<TransformComponent> GetTransform() const { return m_transformComponent; }
		bool IsDead() const { return m_isDead; }

		GameObject(float x = 0, float y = 0);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::shared_ptr<TransformComponent> m_transformComponent;
		std::vector <std::shared_ptr<BaseComponent>> m_components{};

		bool m_isDead{ false };
	};
}
