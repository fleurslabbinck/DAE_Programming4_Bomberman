#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	class HealthComponent : public BaseComponent
	{
	public:
		void InflictDamage();

		explicit HealthComponent(GameObject* pOwner, int maxLives = 3) : BaseComponent(pOwner), m_maxLives{ maxLives } {}
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		const int m_maxLives;
		int m_lives{ m_maxLives };

		void Die();
	};
}
#endif