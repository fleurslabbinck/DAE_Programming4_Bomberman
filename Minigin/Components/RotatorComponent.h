#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include "Components/BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	
	//---------------------------------
	//ROTATORCOMPONENT
	//---------------------------------
	class RotatorComponent : public BaseComponent
	{
	public:
		enum class RotateState {
			Static,
			Rotation
		};

		void Update() override;
		
		void SetRotation(float angle) { m_rotation = angle; };

		explicit RotatorComponent(GameObject* pOwner, float rotation = 0, const RotateState& state = RotateState::Rotation);
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;

	private:
		float m_rotation{};
		RotateState m_rotateState{};

		glm::vec2 RotatePoint(const glm::vec2& pos) const;
	};
}
#endif