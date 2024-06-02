#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"
#include "../BombermanUtil.h"

namespace dae
{
	using Event = int;

	class GameObject;

	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	class HealthComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		void OnNotify(Event event, GameObject* gameObject) override;

		entities::EntityType GetEntityType() const { return m_entityType; }
		int GetLives() const { return m_lives; }

		explicit HealthComponent(GameObject* pOwner, entities::EntityType entityType, int maxLives = 2, bool enemyPlayer = false);
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		bool m_dead{ false };
		bool m_enemyPlayer{ false };
		const entities::EntityType m_entityType;
		const int m_maxLives;
		int m_lives{ m_maxLives };

		void HandleCollision(GameObject* gameObject);
		bool IsGameOver();
		void Respawn();
	};
}
#endif