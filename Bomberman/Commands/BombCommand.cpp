#include "BombCommand.h"

#include "Objects/Components/ColliderComponent.h"
#include "../Components/GridComponent.h"
#include "../Components/BomberComponent.h"

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
		glm::vec2 bottomLeft{ gameObject->GetTransform()->GetLocalPosition() };
		glm::vec2 centeredPos{ collider->GetLocalCenter() };

		gameObject->GetComponent<BomberComponent>()->DropBomb(gameObject->GetParent(), gridComponent->GetGridStartPos(centeredPos));
	}
}