#include "BomberComponent.h"

#include "ServiceLocator.h"
#include "Sound/LoggingSoundSystem.h"
#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Objects/Components/ColliderComponent.h"
#include "BombComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"

namespace dae
{
	BomberComponent::BomberComponent(GameObject* pOwner, Scene& scene)
		: BaseComponent(pOwner), m_scene{ scene }
	{
		
	}

	void BomberComponent::Update()
	{
		float time{ TimeManager::GetInstance().GetDeltaTime() };

		for (Bomb& bomb : m_bombs)
		{
			bomb.timer += time;

			if (bomb.timer >= m_detonationTime)
				ExplodeBomb();
		}
	}

	void BomberComponent::SetPowerUpState(powerUps::PowerUpState powerUpState)
	{
		m_powerUpState = powerUpState;
	}

	void BomberComponent::OnNotify(Event event, GameObject*)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::BOMBS):
			++m_powerUpState.maxBombs;
			break;
		case static_cast<int>(GameEvent::FIRE):
			++m_powerUpState.fire;
			break;
		case static_cast<int>(GameEvent::DETONATOR):
			m_powerUpState.canDetonate = true;
			break;
		}
	}

	void BomberComponent::DropBomb(GameObject* parent, const glm::vec2& pos)
	{
		if (!CanPlaceBomb()) return;

		GameObject* bomb{ m_scene.AddGameObject(std::make_unique<GameObject>("bomb", pos.x, pos.y))};
		bomb->SetParent(parent);

		if (m_powerUpState.maxBombs == 1) bomb->AddComponent<BombComponent>(m_powerUpState.fire);
		else bomb->AddComponent<BombComponent>(m_powerUpState.fire, true);

		ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::PlaceBomb), 10);

		m_bombs.push_back({ bomb });
	}

	void BomberComponent::ExplodeBomb()
	{
		ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::Explosion), 20);

		BombComponent* bombComp{ m_bombs.front().bombObj->GetComponent<BombComponent>()};
		if (bombComp) bombComp->Explode();

		m_bombs.erase(m_bombs.begin());

		m_bombs.erase(std::remove_if(m_bombs.begin(), m_bombs.end(), [](Bomb& bomb)
			{
				BombComponent* otherBombComp{ bomb.bombObj->GetComponent<BombComponent>() };
				if (otherBombComp->ShouldExplode())
				{
					otherBombComp->Explode();
					return true;
				}
				return false;
			}
		), m_bombs.end());
	}

	void BomberComponent::Detonate()
	{
		ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::Explosion), 20);

		for (Bomb& bomb : m_bombs)
		{
			BombComponent* bombComp{ bomb.bombObj->GetComponent<BombComponent>() };
			if (bombComp) bombComp->Explode();
		}

		m_bombs.clear();
	}
}