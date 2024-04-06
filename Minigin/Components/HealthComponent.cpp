#include "HealthComponent.h"
#include "GameObject.h"

#include "SceneManager.h"

namespace dae
{
	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	HealthComponent::HealthComponent(GameObject* pOwner, int maxLives)
		: BaseComponent(pOwner), m_maxLives{ maxLives }
	{
		m_respawnPos = GetOwner()->GetTransform()->GetLocalPosition();
	}

	void HealthComponent::OnNotify(GameEvent event, const GameObject* gameObject)
	{
		switch (event)
		{
		case dae::GameEvent::WALL_DESTROYED:
			Die();
			break;
		case dae::GameEvent::ENEMY_KILLED:
			InflictDamage(gameObject);
			break;
		case dae::GameEvent::PLAYER_DIED:
			InflictDamage(gameObject);
			break;
		}
	}

	void HealthComponent::InflictDamage(const GameObject* gameObject)
	{
		if (m_lives > 1)
		{
			--m_lives;
			Notify(GameEvent::HEALTH_CHANGED, gameObject);
			Respawn();
		}
		else
		{
			Die();
			Notify(GameEvent::GAME_OVER, gameObject);
			SceneManager::GetInstance().SetGameState(SceneManager::GameState::GAME_OVER);
		}
	}

	void HealthComponent::Respawn()
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_respawnPos);
	}

	void HealthComponent::Die()
	{
		GetOwner()->SetDead();
	}
}