#ifndef BOMBERCOMPONENT_H
#define BOMBERCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Scene/Scene.h"

namespace dae
{
	class HealthComponent;

	struct Bomb
	{
		GameObject* bombObj{ nullptr };
		float timer{};
	};

	class BomberComponent final : public BaseComponent
	{
	public:
		void Update();

		bool CanPlaceBomb() const { return m_bombs.size() < m_maxBombs; }
		void DropBomb(GameObject* parent, HealthComponent* healthComp, const glm::vec2& pos);
		void ExplodeBomb();
		void ExplodeBombs();

		explicit BomberComponent(GameObject* pOwner, Scene& scene);
		BomberComponent(const BomberComponent& other) = delete;
		BomberComponent(BomberComponent&& other) = delete;

	private:
		Scene& m_scene;

		uint8_t m_maxBombs{ 1 };
		uint8_t m_fire{ 1 };
		const float m_detonationTime{ 3.f };
		const float m_cooldown{ 0.5f };
		float m_accumulatedTime{};

		std::vector<Bomb> m_bombs{};
	};
}
#endif