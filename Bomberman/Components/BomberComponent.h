#ifndef BOMBERCOMPONENT_H
#define BOMBERCOMPONENT_H

#include "../Minigin/Objects/Components/BaseComponent.h"
#include "../Minigin/Scene/Scene.h"

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