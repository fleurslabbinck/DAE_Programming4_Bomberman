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
		int GetScore() const { return m_score; }

		explicit HealthComponent(GameObject* pOwner, entities::EntityType entityType, int maxLives = 3);
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		bool m_dead{ false };
		const entities::EntityType m_entityType{ entities::EntityType::Bomberman };
		const int m_maxLives;
		int m_lives{ m_maxLives };
		int m_score{};

		glm::vec2 m_respawnPos{};

		void HandleCollision(GameObject* gameObject);
		bool IsGameOver();
		void Respawn();
	};
}
#endif