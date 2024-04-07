#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "BaseComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	class HealthComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		void OnNotify(GameEvent event, GameObject* gameObject);
		void InflictDamage();

		explicit HealthComponent(GameObject* pOwner, int maxLives = 3);
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		const int m_maxLives;
		int m_lives{ m_maxLives };

		glm::vec2 m_respawnPos{};

		void Respawn();
		void Die();
	};
}
#endif