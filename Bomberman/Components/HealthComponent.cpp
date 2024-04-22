#include "HealthComponent.h"

#include "../Minigin/Objects/GameObject.h"
#include "../Minigin/Scene/SceneManager.h"

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
		case GameEvent::PLAYER_HIT:
			Notify(GameEvent::PLAYER_DEATH, gameObject);
			break;
		case GameEvent::ENEMY_HIT:
			Notify(GameEvent::ENEMY_DEATH, gameObject);
			break;
		case GameEvent::WALL_HIT:
			Notify(GameEvent::WALL_DEATH, gameObject);
			break;
		case GameEvent::PLAYER_RESPAWN:
			gameObject->GetComponent<HealthComponent>()->HandleHealth();
			break;
		}
	}

	void HealthComponent::HandleHealth()
	{
		if (m_maxLives > 0)
		{
			--m_lives;
			Notify(GameEvent::HEALTH_CHANGED, GetOwner());
			Respawn();
		}

		if (m_lives < 0)
		{
			Notify(GameEvent::GAME_OVER, GetOwner());
			SceneManager::GetInstance().SetGameState(SceneManager::GameState::GAME_OVER);
		}
	}

	void HealthComponent::Respawn()
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_respawnPos);
	}
}