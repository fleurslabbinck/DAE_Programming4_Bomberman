#pragma once
#include <memory>
#include <vector>
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class Component;

	// todo: this should become final.
	class GameObject final
	{
	public:
		void AddComponent(std::shared_ptr<Component> object);
		void RemoveComponent(std::shared_ptr<Component> object);
		
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

		void FixedUpdate(float deltaTime);
		void Update(float deltaTime);
		void Render() const;

		///void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_texture{};
		std::vector <std::shared_ptr<Component>> m_components{};
	};
}
