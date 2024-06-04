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
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::PlaceBomb), "../Data/Sounds/PlaceBomb.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::Explosion), "../Data/Sounds/Explosion.wav");
		
	}

	void BomberComponent::Update()
	{
		float time{ TimeManager::GetInstance().GetDeltaTime() };

		m_accumulatedTime += time;

		for (Bomb& bomb : m_bombs)
		{
			bomb.timer += time;

			if (bomb.timer >= m_detonationTime)
				ExplodeBomb();
		}
	}

	void BomberComponent::DropBomb(GameObject* parent, const glm::vec2& pos)
	{
		if (!CanPlaceBomb() || m_accumulatedTime < m_cooldown) return;

		GameObject* bomb{ m_scene.AddGameObject(std::make_unique<GameObject>("bomb", pos.x, pos.y))};
		bomb->SetParent(parent);

		if (m_maxBombs == 1) bomb->AddComponent<BombComponent>(m_fire);
		else bomb->AddComponent<BombComponent>(m_fire, true);

		ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::PlaceBomb), 10);

		m_bombs.push_back({ bomb });

		m_accumulatedTime = 0;
	}

	void BomberComponent::ExplodeBomb()
	{
		ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::Explosion), 20);

		BombComponent* bombComp{ m_bombs.front().bombObj->GetComponent<BombComponent>()};
		if (bombComp) bombComp->Explode();

		m_bombs.erase(m_bombs.begin());
	}

	void BomberComponent::ExplodeBombs()
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