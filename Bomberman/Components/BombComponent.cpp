#include "BombComponent.h"

#include <sdl.h>

#include "Render/Renderer.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/GridComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	BombComponent::BombComponent(GameObject* pOwner, uint8_t fire, bool checkForChainExplosion)
		: BaseComponent(pOwner),
		m_colliderComponentBomb{ std::make_unique<ColliderComponent>(pOwner, glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) },
		m_healthComponent{ std::make_unique<HealthComponent>(pOwner, entities::EntityType::Explosion, 1) },
		m_spriteComponent{ std::make_unique<SpriteComponent>(pOwner, "Sprites/Bomb.png", entities::EntityType::Bomb) },
		m_fire{ fire },
		m_checkForChainExplosion{ checkForChainExplosion }
	{
		m_collisionManager.AddCollider(m_colliderComponentBomb.get());

		m_subComponents.push_back(m_colliderComponentBomb.get());
		m_subComponents.push_back(m_healthComponent.get());
		m_subComponents.push_back(m_spriteComponent.get());

		//collider
		constexpr float offset{ 0 };
		m_explosionCollider = { static_cast<float>(constants::GRIDCELL) - offset, static_cast<float>(constants::GRIDCELL) - offset };
	}

	BombComponent::~BombComponent()
	{
		if (m_explode) for (auto& explosion : m_explosions) m_collisionManager.RemoveCollider(explosion.colliderComp.get());
	}

	void BombComponent::Update()
	{
		for (auto& component : m_subComponents) component->Update();

		for (auto& explosion : m_explosions) explosion.colliderComp->Update();

		if (m_explode)
		{
			m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();

			const float maxTime{ 1.f / m_framesPerSecond };

			if (m_accumulatedTime >= maxTime)
			{
				if (!m_oppositeAnim)
				{
					++m_currentFirePhase;
					if (m_currentFirePhase >= m_maxFrames - 1) m_oppositeAnim = true;
				}
				else
				{
					--m_currentFirePhase;
					if (m_currentFirePhase <= 0) KillBomb();
				}
				m_accumulatedTime -= maxTime;
			}
		}
	}

	void BombComponent::Render(const glm::vec2& pos) const
	{
		for (auto& component : m_subComponents) component->Render(pos);

		if (m_explode)
		{
			constexpr int srcGridCell{ constants::GRIDCELL };

			for (auto& explosion : m_explosions)
			{
				SDL_Rect srcRect{ m_currentFirePhase * srcGridCell, explosion.row * srcGridCell, srcGridCell, srcGridCell };
				SDL_Rect dstRect{ static_cast<int>(explosion.pos.x), static_cast<int>(explosion.pos.y), constants::GRIDCELL, constants::GRIDCELL };

				Renderer::GetInstance().RenderTexture(*explosion.renderComp->GetTexture(), srcRect, dstRect);
			}
		}
	}

	void BombComponent::Explode()
	{
		if (m_explode) return;

		m_collisionManager.RemoveCollider(m_colliderComponentBomb.get());
		m_spriteComponent->ToggleVisibility();

		AssembleExplosion();

		for (auto& explosion : m_explosions)
		{
			m_collisionManager.AddCollider(explosion.colliderComp.get());
			explosion.colliderComp->AddObserver(m_healthComponent.get());
		}

		m_explode = true;

		for (GameObject* brick : m_bricks) brick->GetComponent<SpriteComponent>()->SetDead();
		for (GameObject* bomb : m_bombs)
		{
			BombComponent* bombComp{ bomb->GetComponent<BombComponent>() };
			bombComp->SetShouldExplode();
		}
	}

	void BombComponent::AssembleExplosion()
	{
		const glm::vec2 bombPos{ GetOwner()->GetTransform()->GetWorldPosition() };

		Explosion midExplosion{};
		midExplosion.pos = bombPos;
		midExplosion.colliderComp = std::make_unique<ColliderComponent>(GetOwner(), glm::vec2{}, m_explosionCollider.x, m_explosionCollider.y);
		midExplosion.renderComp = std::make_unique<RenderComponent>(GetOwner(), "Sprites/Explosion.png");
		midExplosion.row = 0;
		m_explosions.push_back(std::move(midExplosion));

		bool leftBlocked{ false };
		bool rightBlocked{ false };
		bool upBlocked{ false };
		bool downBlocked{ false };

		for (int i{}; i < m_fire; ++i)
		{
			int count{ i + 1 };

			// left
			if (!leftBlocked) leftBlocked = AddExplosionCollider(count, FireDirection::Left, 1, 2);

			// right
			if (!rightBlocked) rightBlocked = AddExplosionCollider(count, FireDirection::Right, 3, 4);

			// up
			if (!upBlocked) upBlocked = AddExplosionCollider(count, FireDirection::Up, 5, 6);

			// down
			if (!downBlocked) downBlocked = AddExplosionCollider(count, FireDirection::Down, 7, 8);
		}
	}

	bool BombComponent::AddExplosionCollider(int explosionCount, FireDirection dir, uint8_t midRow, uint8_t endRow)
	{
		Explosion explosion{};
		explosion.direction = dir;

		glm::vec2 directionOffset{};

		switch (dir)
		{
		case dae::BombComponent::FireDirection::Left:
			directionOffset = { -explosionCount * constants::GRIDCELL, 0 };
			break;
		case dae::BombComponent::FireDirection::Right:
			directionOffset = { explosionCount * constants::GRIDCELL, 0 };
			break;
		case dae::BombComponent::FireDirection::Up:
			directionOffset = { 0, -explosionCount * constants::GRIDCELL };
			break;
		case dae::BombComponent::FireDirection::Down:
			directionOffset = { 0, explosionCount * constants::GRIDCELL };
			break;
		}

		explosion.pos = GetOwner()->GetTransform()->GetWorldPosition() + directionOffset;
		glm::vec2 colliderPos{ GetOwner()->GetTransform()->GetLocalPosition() + directionOffset };

		if (CollisionManager::GetInstance().CanMove(m_explosions.front().colliderComp.get(), {colliderPos.x + constants::GRIDCELL / 2.f, colliderPos.y + constants::GRIDCELL / 2.f}))
		{
			explosion.colliderComp = std::make_unique<ColliderComponent>(GetOwner(), directionOffset, m_explosionCollider.x, m_explosionCollider.y);
			explosion.renderComp = std::make_unique<RenderComponent>(GetOwner(), "Sprites/Explosion.png");

			if (explosionCount < m_fire) explosion.row = midRow;
			else explosion.row = endRow;

			m_explosions.push_back(std::move(explosion));

			return false;
		}
		else
		{
			GridComponent* gridComp{ GetOwner()->GetParent()->GetComponent<GridComponent>()};

			std::vector<GameObject*> entities{ gridComp->GetEntitiesInCell(colliderPos) };

			for (GameObject* entity : entities)
			{
				HealthComponent* healthComp{ entity->GetComponent<HealthComponent>() };

				if (healthComp != nullptr)
				{
					if (healthComp->GetEntityType() == entities::EntityType::Brick) m_bricks.push_back(entity);
					continue;
				}

				if (m_checkForChainExplosion)
				{
					BombComponent* bombComp{ entity->GetComponent<BombComponent>() };
					if (bombComp != nullptr) m_bombs.push_back(entity);
				}
			}
		}

		return true;
	}

	void BombComponent::KillBomb()
	{
		GetOwner()->SetDead();
	}
}