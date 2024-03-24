#include "HealthComponent.h"

//---------------------------------
//HEALTHCOMPONENT
//---------------------------------

void dae::HealthComponent::InflictDamage()
{
	if (m_lives > 1)
	{
		--m_lives;
	}
	else Die();
}

void dae::HealthComponent::Die()
{

}