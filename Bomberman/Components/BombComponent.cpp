#include "BombComponent.h"

#include <sdl.h>

#include "Render/Renderer.h"
#include "Objects/Components/RenderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	BombComponent::BombComponent(GameObject* pOwner, uint8_t fire)
		: BaseComponent(pOwner),
		m_colliderComponentBomb{ std::make_unique<ColliderComponent>(pOwner, glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) },
		m_healthComponent{ std::make_unique<HealthComponent>(pOwner, entities::EntityType::Explosion, 1) },
		m_spriteComponent{ std::make_unique<SpriteComponent>(pOwner, "Sprites/Bomb.png", entities::EntityType::Bomb) },
		m_fire{ fire }
	{
		m_collisionManager.AddCollider(m_colliderComponentBomb.get());

		m_subComponents.push_back(m_colliderComponentBomb.get());
		m_subComponents.push_back(m_healthComponent.get());
		m_subComponents.push_back(m_spriteComponent.get());

		const glm::vec2 bombPos{ GetOwner()->GetTransform()->GetWorldPosition() };

		//collider
		Explosion midExplosion{};
		midExplosion.pos = bombPos;
		midExplosion.colliderComp = std::make_unique<ColliderComponent>(GetOwner(), glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL));
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
			if (!leftBlocked) leftBlocked = AddExplosion(count, FireDirection::Left, 1, 2);

			// right
			if (!rightBlocked) rightBlocked = AddExplosion(count, FireDirection::Right, 3, 4);

			// up
			if (!upBlocked) upBlocked = AddExplosion(count, FireDirection::Up, 5, 6);

			// down
			if (!downBlocked) downBlocked = AddExplosion(count, FireDirection::Down, 7, 8);
		}
	}

	BombComponent::~BombComponent()
	{
		if (m_explode) for (auto& explosion : m_explosions)
			if (m_collisionManager.HasCollider(explosion.colliderComp.get())) m_collisionManager.RemoveCollider(explosion.colliderComp.get());
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
				m_currentFirePhase += 1;
				m_accumulatedTime -= maxTime;

				if (m_currentFirePhase > 4) KillBomb();
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
		m_collisionManager.RemoveCollider(m_colliderComponentBomb.get());
		m_spriteComponent->ToggleVisibility();

		for (auto& explosion : m_explosions) m_collisionManager.AddCollider(explosion.colliderComp.get());

		m_explode = true;
	}

	bool BombComponent::AddExplosion(int explosionCount, FireDirection dir, uint8_t midRow, uint8_t endRow)
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
			explosion.colliderComp = std::make_unique<ColliderComponent>(GetOwner(), directionOffset, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL));
			explosion.renderComp = std::make_unique<RenderComponent>(GetOwner(), "Sprites/Explosion.png");

			if (explosionCount < m_fire) explosion.row = midRow;
			else explosion.row = endRow;

			m_explosions.push_back(std::move(explosion));

			return false;
		}

		return true;
	}

	void BombComponent::KillBomb()
	{
		GetOwner()->SetDead();
	}
}