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
				ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathTune), 10);
				Notify(static_cast<int>(GameEvent::RESET_LEVEL), nullptr);
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
			ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound), 10);
			m_dead = true;
			break;
		case entities::EntityType::Balloom:
			if (!gameObject->GetComponent<HealthComponent>())
			Notify(static_cast<int>(GameEvent::ENEMY_DEATH), GetOwner());
			break;
		case entities::EntityType::Brick:
			if (gameObject->GetComponent<HealthComponent>()->GetEntityType() == entities::EntityType::Explosion)
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
		}

		if (m_lives < 0)
		{
			Notify(static_cast<int>(GameEvent::GAME_OVER), nullptr);
			return true;
		}

		return false;
	}
}