#ifndef BOMBCOMPONENT_H
#define BOMBCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Objects/Components/RenderComponent.h"
#include "Objects/CollisionManager.h"
#include "Scene/Scene.h"

namespace dae
{
	class ColliderComponent;
	class HealthComponent;
	class SpriteComponent;

	class BombComponent final : public BaseComponent
	{
		enum class FireDirection {
			Left,
			Right,
			Up,
			Down,
			None,
		};

		struct Explosion
		{
			FireDirection direction{ FireDirection::None };
			uint8_t row{};
			glm::vec2 pos{};
			std::unique_ptr<ColliderComponent> colliderComp{ nullptr };
			std::unique_ptr<RenderComponent> renderComp{ nullptr };
		};

	public:
		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void Explode();
		
		void SetShouldExplode() { m_shouldExplode = true; }
		bool ShouldExplode() const { return m_shouldExplode; }

		explicit BombComponent(GameObject* pOwner, uint8_t fire, bool checkForChainExplosion = false);
		~BombComponent() override;
		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;

	private:
		CollisionManager& m_collisionManager{ CollisionManager::GetInstance() };

		std::unique_ptr<ColliderComponent> m_colliderComponentBomb;
		std::unique_ptr<HealthComponent> m_healthComponent;
		std::unique_ptr<SpriteComponent> m_spriteComponent;
		
		bool m_explode{ false };
		bool m_shouldExplode{ false };
		bool m_checkForChainExplosion;

		uint8_t m_fire;
		uint8_t m_currentFirePhase{};
		uint8_t m_maxFrames{ 4 };
		const uint8_t m_framesPerSecond{ 10 };
		float m_accumulatedTime{};
		bool m_oppositeAnim{ false };

		glm::vec2 m_explosionCollider{};
		std::vector<Explosion> m_explosions{};
		std::vector<GameObject*> m_bricks{};
		std::vector<GameObject*> m_bombs{};

		void AssembleExplosion();
		bool AddExplosionCollider(int explosionCount, FireDirection dir, uint8_t midRow, uint8_t endRow);
		void KillBomb();
	};
}
#endif