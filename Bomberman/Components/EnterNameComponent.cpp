#include "EnterNameComponent.h"

#include "Objects/Components/TextComponent.h"
#include "Objects/Components/RenderComponent.h"
#include "Input/InputManager.h"
#include "../BombermanManager.h"

namespace dae
{
	//---------------------------------
	//ENTERNAMECOMPONENT
	//---------------------------------
	EnterNameComponent::EnterNameComponent(GameObject* pOwner, const std::string& fontpath, int fontSize, SDL_Color textColor, SDL_Color shadowColor)
		: BaseComponent(pOwner),
		m_enterNameText{ std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "ENTER NAME", textColor, shadowColor) },
		m_playerName{ std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "...", textColor, shadowColor) }
	{
		m_enterNamePos = { constants::WINDOW_WIDTH / 4.f, constants::WINDOW_HEIGHT / 3.f };
		m_playerNamePos = { constants::WINDOW_WIDTH / 4.f, m_enterNamePos.y + 15.f };

		InputManager::GetInstance().ResetPlayerInput();
	}

	void EnterNameComponent::Update()
	{
		std::string playerInput{ InputManager::GetInstance().GetPlayerInput() };

		if (playerInput != m_playerInput)
		{
			m_playerInput = playerInput;

			m_playerNameUpper = "";

			for (char c : playerInput) m_playerNameUpper += static_cast<char>(std::toupper(c));

			if (m_playerNameUpper.empty()) m_playerNameUpper = " ";

			m_playerName->SetText(m_playerNameUpper);
			m_playerName->Update();
		}
	}

	void EnterNameComponent::Render(const glm::vec2&) const
	{
		m_enterNameText->Render(m_enterNamePos);
		m_playerName->Render(m_playerNamePos);
	}

	void EnterNameComponent::OnNotify(Event event, GameObject*)
	{
		if (event == static_cast<int>(GameEvent::CONTINUE) && !m_playerNameUpper.empty())
		{
			BombermanManager::GetInstance().SetPlayerName(m_playerNameUpper);
			Notify(static_cast<int>(GameEvent::CONTINUE), nullptr);
		}
	}
}