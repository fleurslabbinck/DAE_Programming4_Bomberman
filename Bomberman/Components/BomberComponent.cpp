#include "BomberComponent.h"

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

		m_accumulatedTime += time;

		for (Bomb& bomb : m_bombs)
		{
			bomb.timer += time;

			if (bomb.timer >= m_detonationTime)
				ExplodeBomb();
		}
	}

	void BomberComponent::DropBomb(GameObject* parent, HealthComponent* healthComp, const glm::vec2& pos)
	{
		if (!CanPlaceBomb() || m_accumulatedTime < m_cooldown) return;

		GameObject* bomb{ m_scene.AddGameObject(std::make_unique<GameObject>(pos.x, pos.y)) };
		bomb->AddComponent<BombComponent>(healthComp, m_fire);
		bomb->SetParent(parent);

		m_bombs.push_back({ bomb });

		m_accumulatedTime = 0;
	}

	void BomberComponent::ExplodeBomb()
	{
		BombComponent* bombComp{ m_bombs.front().bombObj->GetComponent<BombComponent>()};
		if (bombComp) bombComp->Explode();

		m_bombs.erase(m_bombs.begin());
	}

	void BomberComponent::ExplodeBombs()
	{
		for (Bomb& bomb : m_bombs)
		{
			BombComponent* bombComp{ bomb.bombObj->GetComponent<BombComponent>() };
			if (bombComp) bombComp->Explode();
		}

		m_bombs.clear();
	}
}