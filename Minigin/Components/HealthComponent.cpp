#include "HealthComponent.h"

namespace dae
{
	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	void HealthComponent::InflictDamage()
	{
		if (m_lives > 1)
		{
			--m_lives;
		}
		else Die();
	}

	void HealthComponent::Die()
	{

	}
}