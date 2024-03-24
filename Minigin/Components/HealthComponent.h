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

		explicit HealthComponent(GameObject* pOwner) : BaseComponent(pOwner) {}
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;

	private:
		const int m_maxLives{ 3 };
		int m_lives{ m_maxLives };

		void Die();
	};
}
#endif