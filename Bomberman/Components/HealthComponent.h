#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "../Minigin/Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

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

		int GetLives() const { return m_lives; }
		int GetScore() const { return m_score; }

		explicit HealthComponent(GameObject* pOwner, int maxLives = 3);
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		const int m_maxLives;
		int m_lives{ m_maxLives };
		int m_score{};

		glm::vec2 m_respawnPos{};

		void HandleHealth();
		void Respawn();
	};
}
#endif