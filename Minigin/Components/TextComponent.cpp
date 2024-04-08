#include "Components/TextComponent.h"
#include "Render/ResourceManager.h"
#include "Render/Renderer.h"
#include <SDL_ttf.h>
#include <sstream>

namespace dae
{
	//---------------------------------
	//TEXTCOMPONENT
	//---------------------------------
	TextComponent::TextComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text)
		: BaseComponent(pOwner), m_text{ text }
	{
		m_font = std::move(dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize));
		m_renderComponent = std::make_unique<RenderComponent>(pOwner, std::make_unique<Texture2D>(GetTexture()));

		m_subComponents.push_back(m_renderComponent.get());
	}

	void TextComponent::Update()
	{
		if (m_needsUpdate) UpdateText();
	}

	void TextComponent::Render(const glm::vec2& pos) const
	{
		const SDL_Rect viewport{ Renderer::GetInstance().GetViewport() };
		const glm::vec2 renderPos{ -viewport.x + pos.x, viewport.y + pos.y };

		m_renderComponent->Render(renderPos);
	}

	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::UpdateText()
	{
		m_renderComponent->SetTexture(std::make_unique<Texture2D>(GetTexture()));
		m_needsUpdate = false;
	}

	SDL_Texture* TextComponent::GetTexture()
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		return texture;
	}
}