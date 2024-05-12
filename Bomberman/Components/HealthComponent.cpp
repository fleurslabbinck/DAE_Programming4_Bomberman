#include "HealthComponent.h"

#include "Objects/GameObject.h"
#include "Scene/SceneManager.h"
#include "ServiceLocator.h"
#include "Sound/SoundSystem.h"
#include "../BombermanManager.h"

namespace dae
{
	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	HealthComponent::HealthComponent(GameObject* pOwner, entities::EntityType entityType, int maxLives)
		: BaseComponent(pOwner), m_entityType{ entityType }, m_maxLives { maxLives }
	{
		m_respawnPos = GetOwner()->GetTransform()->GetLocalPosition();
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathSound), "../Data/Sounds/DieSound.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathTune), "../Data/Sounds/BombermanDies.wav");
	}

	void HealthComponent::OnNotify(Event event, GameObject* gameObject)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::ENTITYOVERLAP):
			HandleCollision(gameObject);
			break;
		case static_cast<int>(GameEvent::PLAYER_RESPAWN):
			if (!IsGameOver())
			{
				ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathTune));
				BombermanManager::GetInstance().HandleGame(static_cast<int>(gameInput::GameInput::LevelReset));
			}
			break;
		}
	}

	void HealthComponent::HandleCollision(GameObject* gameObject)
	{
		if (m_dead) return;

		switch (m_entityType)
		{
		case entities::EntityType::Bomberman:
			Notify(static_cast<int>(GameEvent::PLAYER_DEATH), GetOwner());
			ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound));
			m_dead = true;
			break;
		case entities::EntityType::Balloom:
			if (gameObject->GetComponent<HealthComponent>()->GetEntityType() == entities::EntityType::Bomb)
			Notify(static_cast<int>(GameEvent::ENEMY_DEATH), GetOwner());
			//ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound));
			break;
		case entities::EntityType::Brick:
			if (gameObject->GetComponent<HealthComponent>()->GetEntityType() == entities::EntityType::Bomb)
			Notify(static_cast<int>(GameEvent::BRICK_DEATH), GetOwner());
			//ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound));
		}
	}

	bool HealthComponent::IsGameOver()
	{
		--m_lives;

		if (m_maxLives > 0)
		{
			Notify(static_cast<int>(GameEvent::HEALTH_CHANGED), GetOwner());
			Respawn();
		}

		if (m_lives < 0)
		{
			BombermanManager::GetInstance().HandleGame(static_cast<int>(gameInput::GameInput::GameOver));
			return true;
		}

		return false;
	}

	void HealthComponent::Respawn()
	{
		GetOwner()->GetTransform()->SetLocalPosition(m_respawnPos);
		m_dead = false;
	}
}