#include "MenuControllerComponent.h"

#include "Objects/Components/TextComponent.h"
#include "Objects/Components/RenderComponent.h"

namespace dae
{
	//---------------------------------
	//MENUCONTROLLERCOMPONENT
	//---------------------------------
	MenuControllerComponent::MenuControllerComponent(GameObject* pOwner, const std::string& fontpath, int fontSize)
		: BaseComponent(pOwner),
		m_credits{ std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "RECREATED BY FLEUR SLABBINCK", m_textColor, m_shadowColor) }
	{
		const glm::vec2 menuPos{ constants::WINDOW_WIDTH / 2.f, static_cast<float>(constants::WINDOW_HEIGHT - 90) };
		constexpr float margin{ 7.f };

		m_creditPos = { menuPos.x - m_credits.get()->GetRenderComponent()->GetTexture()->GetSize().x / 2.f, static_cast<float>(constants::WINDOW_HEIGHT - 15) };

		MenuItem single{};
		single.event = GameEvent::START_SINGLE;
		single.textComp = std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "SINGLEPLAYER", m_textColor, m_shadowColor);
		single.dimensions = single.textComp.get()->GetRenderComponent()->GetTexture()->GetSize();
		single.startPos = { menuPos.x - single.dimensions.x / 2.f, menuPos.y };
		m_menuItems.push_back(std::move(single));

		MenuItem coop{};
		coop.event = GameEvent::START_COOP;
		coop.textComp = std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "COOP", m_textColor, m_shadowColor);
		coop.dimensions = coop.textComp.get()->GetRenderComponent()->GetTexture()->GetSize();
		coop.startPos = { menuPos.x - coop.dimensions.x / 2.f, menuPos.y + 1 * (coop.dimensions.y + margin) };
		m_menuItems.push_back(std::move(coop));

		MenuItem pvp{};
		pvp.event = GameEvent::START_PVP;
		pvp.textComp = std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "PVP", m_textColor, m_shadowColor);
		pvp.dimensions = pvp.textComp.get()->GetRenderComponent()->GetTexture()->GetSize();
		pvp.startPos = { menuPos.x - pvp.dimensions.x / 2.f, menuPos.y + 2 * (coop.dimensions.y + margin) };
		m_menuItems.push_back(std::move(pvp));

		MenuItem highScore{};
		highScore.event = GameEvent::HIGHSCORES;
		highScore.textComp = std::make_unique<TextComponent>(pOwner, fontpath, fontSize, "HIGHSCORES", m_textColor, m_shadowColor);
		highScore.dimensions = highScore.textComp.get()->GetRenderComponent()->GetTexture()->GetSize();
		highScore.startPos = { menuPos.x - highScore.dimensions.x / 2.f, menuPos.y + 3 * (coop.dimensions.y + margin) };
		m_menuItems.push_back(std::move(highScore));

		m_menuPointer.currentMenuItem = &m_menuItems.front();
		m_menuPointer.renderComp = std::make_unique<RenderComponent>(pOwner, "MenuPointer.png");
	}

	void MenuControllerComponent::Render(const glm::vec2&) const
	{
		m_credits->Render(m_creditPos);

		const glm::vec2 menuItemPos{ m_menuPointer.currentMenuItem->startPos };
		m_menuPointer.renderComp->Render({ menuItemPos.x - m_menuPointer.offset, menuItemPos.y });

		for (const MenuItem& item : m_menuItems)
		{
			item.textComp->Render(item.startPos);
		}
	}

	void MenuControllerComponent::OnNotify(Event event, GameObject*)
	{
		switch (static_cast<GameEvent>(event))
		{
		case GameEvent::CONTINUE:
			Notify(static_cast<int>(m_menuPointer.currentMenuItem->event), nullptr);
			break;
		case GameEvent::DOWN:
			if (m_menuPointer.menuIdx < m_menuItems.size() - 1) m_menuPointer.currentMenuItem = &m_menuItems[++m_menuPointer.menuIdx];
			break;
		case GameEvent::UP:
			if (m_menuPointer.menuIdx > 0) m_menuPointer.currentMenuItem = &m_menuItems[--m_menuPointer.menuIdx];
			break;
		}
	}
}