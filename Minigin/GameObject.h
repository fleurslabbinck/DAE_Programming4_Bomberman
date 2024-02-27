#pragma once
#include <memory>
#include <vector>
#include "Components.h"

namespace dae
{
	class Texture2D;
	class BaseComponent;

	// todo: this should become final.
	class GameObject final
	{
	public:
		void AddComponent(std::shared_ptr<BaseComponent> object);
		void RemoveComponent(std::shared_ptr<BaseComponent> object);
		
		template <typename C>
		std::shared_ptr<C> GetComponent(size_t occurence = 1) const
		{
			size_t count{};

			for (auto& component : m_components)
			{
				if (std::shared_ptr<C> castedComponent = std::dynamic_pointer_cast<C>(component))
					if (++count == occurence) return castedComponent;
			}
			return nullptr;
		};
		bool HasComponent() const { return m_components.empty(); };

		void Update();
		void Render() const;

		void SetPosition(float x, float y);
		std::shared_ptr<TransformComponent> GetTransform() const { return m_transformComponent; }

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::shared_ptr<TransformComponent> m_transformComponent{};
		std::vector <std::shared_ptr<BaseComponent>> m_components{};
	};
}
