#include "HealthComponent.h"

#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Scene/SceneManager.h"
#include "ServiceLocator.h"
#include "Sound/SoundSystem.h"
#include "../BombermanManager.h"
#include "EnemyComponent.h"
#include "BombComponent.h"

namespace dae
{
	//---------------------------------
	//HEALTHCOMPONENT
	//---------------------------------
	HealthComponent::HealthComponent(GameObject* pOwner, entities::EntityType entityType, int maxLives, bool enemyPlayer)
		: BaseComponent(pOwner), m_entityType{ entityType }, m_maxLives { maxLives }, m_enemyPlayer{ enemyPlayer }
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

		const HealthComponent* gameObjHealthComp{ gameObject->GetComponent<HealthComponent>() };

		switch (m_entityType)
		{
		case entities::EntityType::Bomberman:
			Notify(static_cast<int>(GameEvent::PLAYER_DEATH), GetOwner());
			ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::DeathSound), 10);
			m_dead = true;
			break;
		case entities::EntityType::Balloom:
		case entities::EntityType::Oneal:
		case entities::EntityType::Doll:
		case entities::EntityType::Minvo:
			if (gameObjHealthComp != nullptr && gameObjHealthComp->GetEntityType() == entities::EntityType::Bomberman) Notify(static_cast<int>(GameEvent::PLAYER_DEATH), gameObject);
			else if (gameObject->GetComponent<BombComponent>())
			{
				Notify(static_cast<int>(GameEvent::ENEMY_DEATH), GetOwner());
				if (!m_enemyPlayer) GetOwner()->GetComponent<EnemyComponent>()->Killed();
				else CollisionManager::GetInstance().RemoveCollider(GetOwner()->GetComponent<ColliderComponent>());
			}
			break;
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