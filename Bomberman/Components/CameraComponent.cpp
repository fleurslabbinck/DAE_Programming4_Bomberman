#include "CameraComponent.h"

#include "../Minigin/Objects/GameObject.h"
#include "../Minigin/Render/Renderer.h"
#include "../Bomberman/BombermanUtil.h"

namespace dae
{
	CameraComponent::CameraComponent(GameObject* pOwner, int playfieldWidth, int minBorder, int maxBorder)
		: BaseComponent(pOwner), m_minBorder{ minBorder }, m_maxBorder{ maxBorder }
	{
		m_viewport.w = playfieldWidth;
		m_viewport.h = constants::WINDOW_HEIGHT;

		m_clampOffset = m_viewport.h / 2;
	}

	void CameraComponent::FollowPlayer()
	{
		const float playerPos{ GetOwner()->GetTransform()->GetWorldPosition().x + constants::GRIDCELL / 2.f };
		m_viewport.x = static_cast<int>(playerPos) - m_clampOffset;

		ClampPlayer();

		m_viewport.x = -m_viewport.x;
		Renderer::GetInstance().SetViewport(m_viewport);
	}

	void CameraComponent::ClampPlayer()
	{
		if (m_viewport.x < m_minBorder) m_viewport.x = m_minBorder;
		else if (m_viewport.x > m_maxBorder) m_viewport.x = m_maxBorder;
	}
}