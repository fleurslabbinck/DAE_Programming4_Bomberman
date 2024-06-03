#include "SpriteComponent.h"

#include "Render/Renderer.h"
#include "TimeManager.h"
#include "Objects/GameObject.h"
#include "ServiceLocator.h"
#include "Sound/LoggingSoundSystem.h"

namespace dae
{
	SpriteComponent::SpriteComponent(GameObject* pOwner, const std::string& filename, entities::EntityType type)
		: BaseComponent(pOwner), m_renderComponent{ std::make_unique<RenderComponent>(pOwner, filename) }
	{
		m_sprite.type = type;

		if (m_sprite.type == entities::EntityType::Bomberman)
		{
			m_sprite.cols = 7;
			m_sprite.rows = 3;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.framesPerSecond = 10;
			m_sprite.deathFramesPerSecond = 3;

			m_sprite.hasDirection = true;
			m_sprite.startFrameLeft = { 0, 0 };
			m_sprite.startFrameRight = { 0, 1 };
			m_sprite.startFrameDown = { m_sprite.movementFrames, 0 };
			m_sprite.startFrameUp = { m_sprite.movementFrames, 1 };
		}

		if (m_sprite.type == entities::EntityType::Balloom || m_sprite.type == entities::EntityType::Oneal || m_sprite.type == entities::EntityType::Doll || m_sprite.type == entities::EntityType::Minvo)
		{
			m_sprite.cols = 6;
			m_sprite.rows = 2;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.framesPerSecond = 3;
			m_sprite.deathFramesPerSecond = 3;

			m_sprite.hasDirection = true;
			m_sprite.startFrameRight = { 0, 0 };
			m_sprite.startFrameLeft = { m_sprite.movementFrames, 0 };
			m_sprite.startFrameDown = { 0, 0 };
			m_sprite.startFrameUp = { m_sprite.movementFrames, 0 };

			m_sprite.enemy = true;
		}

		if (m_sprite.type == entities::EntityType::Bomb)
		{
			m_sprite.cols = 3;
			m_sprite.rows = 1;
			m_sprite.deathFrames = 0;
			m_sprite.framesPerSecond = 3;

			m_sprite.hasDirection = false;
		}

		if (m_sprite.type == entities::EntityType::Brick)
		{
			m_sprite.cols = 7;
			m_sprite.rows = 1;
			m_sprite.deathFrames = m_sprite.cols;
			m_sprite.deathFramesPerSecond = 10;

			m_sprite.hasDirection = false;
		}

		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::HorizontalStep), "../Data/Sounds/HorizontalStep.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::VerticalStep), "../Data/Sounds/VerticalStep.wav");
	}

	void SpriteComponent::Update()
	{
		if (m_dead) AnimateDying();
		if (m_sprite.type == entities::EntityType::Bomb) AnimateMovement();
	}

	void SpriteComponent::Render(const glm::vec2& pos) const
	{
		if (!m_isVisible) return;

		constexpr int srcGridCell{ constants::GRIDCELL };

		SDL_Rect srcRect{ m_currentIndex * srcGridCell, m_startFrameIndex.rowIdx * srcGridCell, srcGridCell, srcGridCell };
		SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), constants::GRIDCELL, constants::GRIDCELL };

		Renderer::GetInstance().RenderTexture(*m_renderComponent->GetTexture(), srcRect, dstRect);
	}

	void SpriteComponent::OnNotify(Event event, GameObject*)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::PLAYER_DEATH):
			if (m_sprite.type != entities::EntityType::Bomberman) return;
			SetDead();
			break;
		case static_cast<int>(GameEvent::ENEMY_DEATH):
			if (!m_sprite.enemy) return;
			SetDead();
			break;
		}
	}

	void SpriteComponent::SetDirection(const glm::vec2& direction)
	{
		if (m_sprite.hasDirection)
		{
			if (direction.x < 0) m_startFrameIndex = m_sprite.startFrameLeft;
			else if (direction.x > 0) m_startFrameIndex = m_sprite.startFrameRight;
			if (direction.y > 0) m_startFrameIndex = m_sprite.startFrameDown;
			else if (direction.y < 0) m_startFrameIndex = m_sprite.startFrameUp;

			m_lastDirection = direction;
		}
	}

	void SpriteComponent::AnimateMovement()
	{
		m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();
		const float maxTime{ 1.f / m_sprite.framesPerSecond };

		if (m_accumulatedTime >= maxTime)
		{
			m_currentIndex = m_startFrameIndex.colIdx + (m_currentIndex + 1) % m_sprite.movementFrames;
			m_accumulatedTime -= maxTime;

			PlayStepSound();
		}
	}

	void SpriteComponent::AnimateDying()
	{
		m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();
		const float maxTime{ 1.f / m_sprite.deathFramesPerSecond };

		if (m_accumulatedTime >= maxTime)
		{
			m_currentIndex = m_startFrameIndex.colIdx + (m_currentIndex + 1);
			m_accumulatedTime -= maxTime;

			if (m_currentIndex >= m_sprite.deathFrames)
			{
				GameObject* owner{ GetOwner() };

				switch (m_sprite.type)
				{
				case entities::EntityType::Bomberman:
					Notify(static_cast<int>(GameEvent::PLAYER_RESPAWN), owner);
					m_dead = false;
					m_startFrameIndex = m_sprite.startFrameLeft;
					break;
				case entities::EntityType::Balloom:
				case entities::EntityType::Oneal:
				case entities::EntityType::Doll:
				case entities::EntityType::Minvo:
					Notify(static_cast<int>(GameEvent::SCORE_CHANGED), owner);
					owner->SetDead();
					break;
				case entities::EntityType::Brick:
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
		m_lastDirection = {};
	}

	void SpriteComponent::PlayStepSound()
	{
		if (m_sprite.type != entities::EntityType::Bomberman) return;

		m_stepCount = ++m_stepCount % m_maxStepSkip;

		if (m_stepCount == m_maxStepSkip - 1)
		{
			if (m_lastDirection.x * m_lastDirection.x > 0.f) ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::HorizontalStep), 10);
			if (m_lastDirection.y * m_lastDirection.y > 0.f) ServiceLocator::GetSoundSystem().PlaySoundFX(static_cast<int>(sound::SoundId::VerticalStep), 10);
		}
	}
}