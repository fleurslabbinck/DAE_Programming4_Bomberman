#ifndef BOMBERCOMPONENT_H
#define BOMBERCOMPONENT_H

#include "Components/BaseComponent.h"
#include "Scene.h"

namespace dae
{
	class HealthComponent;

	class BomberComponent final : public BaseComponent
	{
	public:
		void DropBomb(GameObject* parent, HealthComponent* healthComp, const glm::vec2& pos);

		explicit BomberComponent(GameObject* pOwner, Scene& scene);
		BomberComponent(const BomberComponent& other) = delete;
		BomberComponent(BomberComponent&& other) = delete;

	private:
		Scene& m_scene;
	};
}

#endif