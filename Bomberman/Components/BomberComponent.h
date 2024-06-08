#ifndef BOMBERCOMPONENT_H
#define BOMBERCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Observer.h"
#include "Scene/Scene.h"
#include "../BombermanUtil.h"

namespace dae
{
	class HealthComponent;

	class BomberComponent final : public BaseComponent, public Observer
	{
	public:
		struct Bomb
		{
			GameObject* bombObj{ nullptr };
			float timer{};
		};

		void Update() override;

		void OnNotify(Event event, GameObject* gameObject) override;

		void SetPowerUpState(powerUps::PowerUpState powerUpState);

		bool CanPlaceBomb() const { return m_bombs.size() < m_powerUpState.maxBombs; }
		void DropBomb(GameObject* parent, const glm::vec2& pos);
		void Detonate();
		bool CanDetonateBombs() const { return m_powerUpState.canDetonate; }

		explicit BomberComponent(GameObject* pOwner, Scene& scene);
		BomberComponent(const BomberComponent& other) = delete;
		BomberComponent(BomberComponent&& other) = delete;

	private:
		Scene& m_scene;

		powerUps::PowerUpState m_powerUpState{};
		const float m_detonationTime{ 3.f };

		std::vector<Bomb> m_bombs{};

		void ExplodeBomb();
	};
}
#endif