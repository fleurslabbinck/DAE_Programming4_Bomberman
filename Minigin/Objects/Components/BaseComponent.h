#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	//---------------------------------
	//BASE COMPONENT
	//---------------------------------
	class BaseComponent
	{
		GameObject* m_pOwner;
	public:
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(const glm::vec2& pos) const;

		virtual void SetDelete() { m_delete = true; }
		virtual bool ShouldBeDeleted() const { return m_delete; }
		
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		virtual BaseComponent& operator=(const BaseComponent& other) = delete;
		virtual BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		std::vector<BaseComponent*> m_subComponents{};

		bool m_delete{ false };

		explicit BaseComponent(GameObject* pOwner) : m_pOwner{ pOwner } {}
		GameObject* GetOwner() const { return m_pOwner; }
	};
}
#endif