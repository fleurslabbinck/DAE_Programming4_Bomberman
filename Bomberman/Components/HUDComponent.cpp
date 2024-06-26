#include "HUDComponent.h"

#include "Render/Renderer.h"
#include "Objects/GameObject.h"
#include "Objects/Components/TextComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ScoreComponent.h"
#include "../BombermanUtil.h"

namespace dae
{
	HUDComponent::HUDComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string&)
		: BaseComponent(pOwner)
	{
		constexpr float offset{ 5.f };

		m_livesMessage = HUDMessage{};
		m_livesMessage.value = GetLives(GetOwner());
		m_livesMessage.text = "LIVES: ";
		m_livesMessage.textComponent = std::make_unique<TextComponent>(pOwner, fontPath, fontSize, m_livesMessage.text);
		m_livesMessage.position.x = offset;
		m_livesMessage.position.y = offset;
		m_livesMessage.SetMessage();

		m_scoreMessage = HUDMessage{};
		m_scoreMessage.value = GetScore(GetOwner());
		m_scoreMessage.text = "SCORE: ";
		m_scoreMessage.textComponent = std::make_unique<TextComponent>(pOwner, fontPath, fontSize, m_scoreMessage.text);
		m_scoreMessage.position.x = offset;
		m_scoreMessage.position.y = 2 * offset + m_scoreMessage.textComponent->GetRenderComponent()->GetTexture()->GetSize().y;
		m_scoreMessage.SetMessage();

		m_subComponents.push_back(m_livesMessage.textComponent.get());
		m_subComponents.push_back(m_scoreMessage.textComponent.get());
	}

	void HUDComponent::OnNotify(Event event, GameObject* gameObject)
	{
		switch (event)
		{
		case static_cast<int>(GameEvent::HEALTH_CHANGED):
			m_livesMessage.value = GetLives(gameObject);
			m_livesMessage.SetMessage();
			break;
		case static_cast<int>(GameEvent::SCORE_CHANGED):
			m_scoreMessage.value = GetScore(gameObject);
			m_scoreMessage.SetMessage();
			break;
		}
	}

	void HUDComponent::Render(const glm::vec2&) const
	{
		const SDL_Rect viewport{ Renderer::GetInstance().GetViewport() };

		const glm::vec2 renderPosLives{ -viewport.x + m_livesMessage.position.x, viewport.y + m_livesMessage.position.y };
		m_livesMessage.textComponent->GetRenderComponent()->Render(renderPosLives);

		const glm::vec2 renderPosScore{ -viewport.x + m_scoreMessage.position.x, viewport.y + m_scoreMessage.position.y };
		m_scoreMessage.textComponent->GetRenderComponent()->Render(renderPosScore);
	}

	int HUDComponent::GetLives(GameObject* gameObject) const
	{
		return gameObject->GetComponent<HealthComponent>()->GetLives();
	}

	int HUDComponent::GetScore(GameObject* gameObject) const
	{
		return gameObject->GetComponent<ScoreComponent>()->GetTotalScore();
	}

	void HUDComponent::HUDMessage::SetMessage()
	{
		textComponent->SetText(text + std::to_string(value));
	}
}