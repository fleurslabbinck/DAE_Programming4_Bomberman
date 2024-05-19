#ifndef BOMBCOMPONENT_H
#define BOMBCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Scene/Scene.h"

namespace dae
{
	class ColliderComponent;
	class HealthComponent;
	class SpriteComponent;

	class BombComponent final : public BaseComponent
	{
	public:
		void Explode();

		explicit BombComponent(GameObject* pOwner, HealthComponent* bombermanHealthComp, uint8_t fire);
		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;

	private:
		std::unique_ptr<ColliderComponent> m_colliderComponent;
		std::unique_ptr<HealthComponent> m_healthComponent;
		std::unique_ptr<SpriteComponent> m_spriteComponentBomb;
		std::unique_ptr<SpriteComponent> m_spriteComponentExplosion;

		uint8_t m_fire;
	};
}
#endif