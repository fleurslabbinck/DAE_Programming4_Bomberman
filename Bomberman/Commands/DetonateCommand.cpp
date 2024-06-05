#include "DetonateCommand.h"

#include "Objects/Components/ColliderComponent.h"
#include "../Components/BomberComponent.h"

namespace dae
{
	//---------------------------------
	// DETONATECOMMAND
	//---------------------------------
	void DetonateCommand::Execute()
	{
		GameObject* gameObject{ GetGameObject() };
		BomberComponent* bomberComponent{ gameObject->GetComponent<BomberComponent>() };

		if (bomberComponent->CanDetonateBombs()) bomberComponent->Detonate();
	}
}