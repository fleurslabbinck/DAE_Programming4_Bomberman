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

	void HealthComponent::OnNotify(GameEvent event, GameObject* gameObject)
	{
		switch (event)
		{
		case dae::GameEvent::WALL_DESTROYED:
			gameObject->SetDead();
			break;
		case dae::GameEvent::ENEMY_KILLED:
			gameObject->SetDead();
			break;
		case dae::GameEvent::PLAYER_DIED:
			gameObject->GetComponent<HealthComponent>()->InflictDamage();
			break;
		}
	}

	void HealthComponent::InflictDamage()
	{
		if (m_lives > 0)
		{
			--m_lives;
			Notify(GameEvent::HEALTH_CHANGED, GetOwner());
			Respawn();
		}
		else
		{
			Notify(GameEvent::GAME_OVER, GetOwner());
			SceneManager::GetInstance().SetGameState(SceneManager::GameState::GAME_OVER);
			Die();
		}
	}

	void HealthComponent::Respawn()
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_respawnPos);
	}

	void HealthComponent::Die()
	{
		GetOwner()->SetDead();
		Notify(GameEvent::SCORE_CHANGED, GetOwner());
	}
}