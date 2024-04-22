#include "TextComponent.h"
#include "Render/Resources/ResourceManager.h"
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
		const SDL_Color colorFrontText = { 255,255,255,255 };
		const SDL_Color colorShadow = { 0,0,0,255 };
		const int shadowOffsetX{ 1 }, shadowOffsetY{ 1 };

		// shadow text
		const auto shadowSurf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), colorShadow);
		if (shadowSurf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto shadowTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), shadowSurf);
		if (shadowTexture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		// main text
		const auto mainSurf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), colorFrontText);
		if (mainSurf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto mainTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), mainSurf);
		if (mainTexture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_Rect mainRect{};
		SDL_QueryTexture(mainTexture, nullptr, nullptr, &mainRect.w, &mainRect.h);

		// target surface for combining
		SDL_Surface* combinedSurf = SDL_CreateRGBSurfaceWithFormat(0, mainRect.w + shadowOffsetX, mainRect.h + shadowOffsetY, 32, SDL_PIXELFORMAT_RGBA8888);
		if(combinedSurf == nullptr)
		{
			throw std::runtime_error(std::string("Render combined surface failed: ") + SDL_GetError());
		}

		SDL_Rect shadowRect{ shadowOffsetX, shadowOffsetY, mainRect.w, mainRect.h };
		SDL_BlitSurface(shadowSurf, nullptr, combinedSurf, &shadowRect);
		SDL_BlitSurface(mainSurf, nullptr, combinedSurf, & mainRect);

		SDL_Texture* combinedTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), combinedSurf);
		if (combinedTexture == nullptr)
		{
			throw std::runtime_error(std::string("Create combined texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(shadowSurf);
		SDL_FreeSurface(mainSurf);
		SDL_FreeSurface(combinedSurf);

		return combinedTexture;
	}
}