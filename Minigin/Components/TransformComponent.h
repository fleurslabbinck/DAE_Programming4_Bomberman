#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	//---------------------------------
	//TRANSFORMCOMPONENT
	//---------------------------------
	class TransformComponent : public BaseComponent
	{
	public:
		void Translate(const glm::vec2& offset);
		void UpdateWorldPosition();
		void SetLocalPosition(const glm::vec2& pos);
		void SetPositionDirty();
		bool IsPositionDirty() const { return m_positionIsDirty; }
		glm::vec2 GetLocalPosition() { return m_localPosition; }
		glm::vec2 GetWorldPosition();

		explicit TransformComponent(GameObject* pOwner, float x = 0, float y = 0) : BaseComponent(pOwner), m_localPosition{ x, y } {}
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;

	private:
		glm::vec2 m_localPosition{};
		glm::vec2 m_worldPosition{};

		bool m_positionIsDirty{ true };
	};
}
#endif