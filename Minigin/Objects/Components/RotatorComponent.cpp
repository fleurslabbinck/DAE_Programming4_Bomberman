#include "RotatorComponent.h"
#include "../GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace dae
{
	//---------------------------------
	//ROTATORCOMPONENT
	//---------------------------------
	RotatorComponent::RotatorComponent(GameObject* pOwner, float rotation, const RotateState& state)
		: BaseComponent(pOwner), m_rotation{ rotation }, m_rotateState{ state }
	{
	
	}

	void RotatorComponent::Update()
	{
		if (m_rotateState == RotateState::Rotation)
		{
			dae::TransformComponent* transform = GetOwner()->GetTransform();

			const glm::vec2 localPos{ transform->GetLocalPosition() };

			const glm::vec2 rotatedPos{ RotatePoint(localPos) };

			transform->SetLocalPosition(rotatedPos);
		}
	}

	glm::vec2 RotatorComponent::RotatePoint(const glm::vec2& pos) const
	{
		const float radians = glm::radians(m_rotation);

		const glm::mat3 rotationMatrix = glm::mat3(
			glm::vec3(cos(radians), -sin(radians), 0.0f),
			glm::vec3(sin(radians), cos(radians), 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));

		const glm::vec3 newPos{ rotationMatrix * glm::vec3{ pos.x, pos.y, 0 } };

		return { newPos.x, newPos.y };
	}
}