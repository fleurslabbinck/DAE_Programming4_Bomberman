#include "SpriteComponent.h"

#include "Render/Renderer.h"
#include "../Minigin/TimeManager.h"
#include "../Minigin/Objects/GameObject.h"
#include "../Bomberman/BombermanUtil.h"

namespace dae
{
	SpriteComponent::SpriteComponent(GameObject* pOwner, const std::string& filename, SpriteType type, int score)
		: BaseComponent(pOwner), m_renderComponent{ std::make_unique<RenderComponent>(pOwner, filename) }
	{
		m_sprite.type = type;

		if (m_sprite.type == SpriteType::BOMBERMAN)
		{
			m_sprite.cols = 7;
			m_sprite.rows = 3;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.framesPerSecond = 10;

			m_sprite.hasDirection = true;
			m_sprite.startFrameLeft = { 0, 0 };
			m_sprite.startFrameRight = { 0, 1 };
			m_sprite.startFrameDown = { m_sprite.movementFrames, 0 };
			m_sprite.startFrameUp = { m_sprite.movementFrames, 1 };
		}

		if (m_sprite.type == SpriteType::BALLOOM || m_sprite.type == SpriteType::ONEAL || m_sprite.type == SpriteType::DOLL || m_sprite.type == SpriteType::MINVO)
		{
			m_sprite.cols = 6;
			m_sprite.rows = 2;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.framesPerSecond = 5;

			m_sprite.hasDirection = true;
			m_sprite.startFrameRight = { 0, 0 };
			m_sprite.startFrameLeft = { m_sprite.movementFrames, 0 };
			m_sprite.startFrameDown = { 0, 0 };
			m_sprite.startFrameUp = { m_sprite.movementFrames, 0 };

			m_sprite.enemy = true;
			m_sprite.score = score;
		}

		if (m_sprite.type == SpriteType::BOMB)
		{
			m_sprite.cols = 3;
			m_sprite.rows = 1;
			m_sprite.deathFrames = 0;
			m_sprite.framesPerSecond = 5;

			m_sprite.hasDirection = false;
		}

		if (m_sprite.type == SpriteType::WALL)
		{
			m_sprite.cols = 7;
			m_sprite.rows = 1;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.framesPerSecond = 5;

			m_sprite.hasDirection = false;
		}
	}

	void SpriteComponent::Update()
	{
		if (m_dead) AnimateDying();
	}

	void SpriteComponent::Render(const glm::vec2& pos) const
	{
		constexpr int srcGridCell{ constants::GRIDCELL };

		SDL_Rect srcRect{ m_currentIndex * srcGridCell, m_startFrameIndex.rowIdx * srcGridCell, srcGridCell, srcGridCell };
		SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), constants::GRIDCELL, constants::GRIDCELL };

		Renderer::GetInstance().RenderTexture(*m_renderComponent->GetTexture(), srcRect, dstRect);
	}

	void SpriteComponent::OnNotify(GameEvent event, GameObject*)
	{
		switch (event)
		{
		case dae::GameEvent::PLAYER_DEATH:
			if (m_sprite.type != SpriteType::BOMBERMAN) return;
			SetDead();
			break;
		case dae::GameEvent::ENEMY_DEATH:
			if (!m_sprite.enemy) return;
			SetDead();
			break;
		case dae::GameEvent::WALL_DEATH:
			if (m_sprite.type != SpriteType::WALL) return;
			SetDead();
			break;
		}
	}

	void SpriteComponent::SetDirection(const glm::vec2& direction)
	{
		if (m_sprite.hasDirection)
		{
			if (direction.x < 0) m_startFrameIndex = m_sprite.startFrameLeft;
			if (direction.x > 0) m_startFrameIndex = m_sprite.startFrameRight;
			if (direction.y > 0) m_startFrameIndex = m_sprite.startFrameDown;
			if (direction.y < 0) m_startFrameIndex = m_sprite.startFrameUp;
		}
	}

	void SpriteComponent::AnimateMovement()
	{
		if (m_sprite.type == SpriteType::BOMB) return;

		m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();
		const float maxTime{ 1.f / m_sprite.framesPerSecond };

		if (m_accumulatedTime >= maxTime)
		{
			m_currentIndex = m_startFrameIndex.colIdx + (m_currentIndex + 1) % m_sprite.movementFrames;
			m_accumulatedTime -= maxTime;
		}
	}

	void SpriteComponent::AnimateDying()
	{
		m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();
		constexpr float maxTime{ 1.f / 3 };

		if (m_accumulatedTime >= maxTime)
		{
			m_currentIndex = m_startFrameIndex.colIdx + (m_currentIndex + 1);
			m_accumulatedTime -= maxTime;

			if (m_currentIndex >= m_sprite.deathFrames)
			{
				GameObject* owner{ GetOwner() };

				switch (m_sprite.type)
				{
				case SpriteType::BOMBERMAN:
					Notify(GameEvent::PLAYER_RESPAWN, owner);
					m_dead = false;
					m_startFrameIndex = m_sprite.startFrameLeft;
					break;
				case SpriteType::BALLOOM:
					Notify(GameEvent::SCORE_CHANGED, owner);
					owner->SetDead();
					break;
				case SpriteType::ONEAL:
					Notify(GameEvent::SCORE_CHANGED, owner);
					owner->SetDead();
					break;
				case SpriteType::DOLL:
					Notify(GameEvent::SCORE_CHANGED, owner);
					owner->SetDead();
					break;
				case SpriteType::MINVO:
					Notify(GameEvent::SCORE_CHANGED, owner);
					owner->SetDead();
					break;
				case SpriteType::WALL:
					owner->SetDead();
					break;
				}
			}
		}
	}

	void SpriteComponent::SetDead()
	{
		m_dead = true;
		m_startFrameIndex.colIdx = 0;
		m_startFrameIndex.rowIdx = m_sprite.rows - 1;
	}
}