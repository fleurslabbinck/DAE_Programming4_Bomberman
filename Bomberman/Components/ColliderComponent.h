#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../Minigin/Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//COLLISIONCOMPONENT
	//---------------------------------
	class ColliderComponent final : public BaseComponent, public Observer
	{
	public:
		enum class EntityType {
			Player,
			Enemy,
			Block,
			Bomb,
			Wall,
		};

		explicit ColliderComponent(GameObject* pOwner, EntityType entityType);
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;

	private:
		EntityType m_entityType;
		
	};
}
#endif