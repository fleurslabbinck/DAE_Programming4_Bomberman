#include "SpriteComponent.h"

#include "BombermanUtil.h"
#include "Render/Renderer.h"
#include "TimeManager.h"

namespace dae
{
	SpriteComponent::SpriteComponent(GameObject* pOwner, const std::string& filename, int cols, int rows, int movementFrames, int deathFrames)
		: BaseComponent(pOwner), m_renderComponent{ std::make_unique<RenderComponent>(pOwner, filename) }, m_cols{ cols }, m_rows{ rows }, m_movementFrames{ movementFrames }, m_deathFrames{ deathFrames }
	{
		if (m_rows == 3)
		{
			m_startFrameLeft = { 0, 0 };
			m_startFrameRight = { 0, 1 };
			m_startFrameDown = { m_movementFrames, 0 };
			m_startFrameUp = { m_movementFrames, 1 };

			m_startFrameIndex = m_startFrameRight;
			m_framesPerSecond *= 2;
		}
		else
		{
			m_startFrameRight = { 0, 0 };
			m_startFrameLeft = { m_movementFrames, 0 };
			m_startFrameDown = { 0, 0 };
			m_startFrameUp = { m_movementFrames, 0 };

			m_startFrameIndex = m_startFrameLeft;
		}


		m_subComponents.push_back(m_renderComponent.get());
	}

	void SpriteComponent::Update()
	{
		if (m_dead) AnimateDying();
	}

	void SpriteComponent::Render(const glm::vec2& pos) const
	{
		//const glm::vec2 dimensions{ m_renderComponent->GetTexture()->GetSize() };
		constexpr int srcGridCell{ GRIDCELL / WINDOW_SCALE };

		SDL_Rect srcRect{ m_currentIndex * srcGridCell, m_startFrameIndex.rowIdx * srcGridCell, srcGridCell, srcGridCell };
		SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), GRIDCELL, GRIDCELL };

		Renderer::GetInstance().RenderTexture(*m_renderComponent->GetTexture(), srcRect, dstRect);
	}

	void SpriteComponent::SetDirection(const glm::vec2& direction)
	{
		if (direction.x < 0) m_startFrameIndex = m_startFrameLeft;
		if (direction.x > 0) m_startFrameIndex = m_startFrameRight;
		if (direction.y > 0) m_startFrameIndex = m_startFrameDown;
		if (direction.y < 0) m_startFrameIndex = m_startFrameUp;
	}

	void SpriteComponent::AnimateMovement()
	{
		m_accumulatedTime += TimeManager::GetInstance().GetDeltaTime();

		if (m_accumulatedTime >= 1.f / m_framesPerSecond)
		{
			m_currentIndex = m_startFrameIndex.colIdx + (m_currentIndex + 1) % m_movementFrames;
			m_accumulatedTime = 0;
		}
	}

	void SpriteComponent::AnimateDying()
	{

	}
}