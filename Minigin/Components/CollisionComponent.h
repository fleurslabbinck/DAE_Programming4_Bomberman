#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//COLLISIONCOMPONENT
	//---------------------------------
	class CollisionComponent : public BaseComponent
	{
	public:
		enum class EntityType {
			Player,
			Enemy,
			Block,
			Bomb,
		};

		EntityType GetEntityType() { return m_entityType; }

		explicit CollisionComponent(GameObject* pOwner, int width, int height) : BaseComponent(pOwner), m_entityType{ EntityType::Player }, m_width { widht }, m_height{ height } {}
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;

	private:
		EntityType m_entityType{};
		const int m_width;
		const int m_height;

		void HandleCollision();
	};
}
#endif