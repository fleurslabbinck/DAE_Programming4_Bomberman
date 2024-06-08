#include "BombCommand.h"

#include "Objects/Components/ColliderComponent.h"
#include "../Components/GridComponent.h"
#include "../Components/BomberComponent.h"
#include "../Components/BombComponent.h"

namespace dae
{
	//---------------------------------
	// BOMBCOMMAND
	//---------------------------------
	void BombCommand::Execute()
	{
		GameObject* gameObject{ GetGameObject() };
		GridComponent* gridComponent{ gameObject->GetParent()->GetComponent<GridComponent>() };
		const ColliderComponent* collider{ gameObject->GetComponent<ColliderComponent>() };

		// get current pos
		glm::vec2 centeredPos{ collider->GetLocalCenter() };

		std::vector<GameObject*> entities{ gridComponent->GetEntitiesInCell(centeredPos) };

		for (const GameObject* entity : entities) if (entity->GetComponent<BombComponent>()) return;

		gameObject->GetComponent<BomberComponent>()->DropBomb(gameObject->GetParent(), gridComponent->GetGridStartPos(centeredPos));
	}
}