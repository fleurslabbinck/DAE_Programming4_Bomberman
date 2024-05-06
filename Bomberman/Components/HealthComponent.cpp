#include "HealthComponent.h"

#include "../Minigin/Objects/GameObject.h"
#include "../Minigin/Scene/SceneManager.h"
#include "../Minigin/ServiceLocator.h"
#include "../Minigin/Sound/SoundSystem.h"
#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	HealthComponent::HealthComponent(GameObject* pOwner, int maxLives)
		: BaseComponent(pOwner), m_maxLives{ maxLives }
	{
		m_respawnPos = GetOwner()->GetTransform()->GetLocalPosition();
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathSound), "../Data/Sounds/DieSound.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathTune), "../Data/Sounds/BombermanDies.wav");
	}

	void HealthComponent::OnNotify(Event event, GameObject* gameObject)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::PLAYER_HIT):
			Notify(static_cast<int>(GameEvent::PLAYER_DEATH), gameObject);
			ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound));
			break;
		case static_cast<int>(GameEvent::ENEMY_HIT):
			Notify(static_cast<int>(GameEvent::ENEMY_DEATH), gameObject);
			break;
		case static_cast<int>(GameEvent::WALL_HIT):
			Notify(static_cast<int>(GameEvent::WALL_DEATH), gameObject);
			break;
		case static_cast<int>(GameEvent::PLAYER_RESPAWN):
			gameObject->GetComponent<HealthComponent>()->HandleHealth();
			ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathTune));
			break;
		}
	}

	void HealthComponent::HandleHealth()
	{
		if (m_maxLives > 0)
		{
			--m_lives;
			Notify(static_cast<int>(GameEvent::HEALTH_CHANGED), GetOwner());
			Respawn();
		}

		if (m_lives < 0)
		{
			Notify(static_cast<int>(GameEvent::GAME_OVER), GetOwner());
			SceneManager::GetInstance().SetGameState(SceneManager::GameState::GAME_OVER);
		}
	}

	void HealthComponent::Respawn()
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_respawnPos);
	}
}