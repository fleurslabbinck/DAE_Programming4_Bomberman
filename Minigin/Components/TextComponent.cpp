#include "Components/TextComponent.h"
#include "Render/ResourceManager.h"
#include "Render/Renderer.h"
#include <SDL_ttf.h>
#include <sstream>

//---------------------------------
//TEXTCOMPONENT
//---------------------------------
void dae::TextComponent::Update()
{
	if (m_needsUpdate) UpdateText();
}

void dae::TextComponent::Initialize(const std::string& fontPath, int fontSize, const std::string& text)
{
	m_font = std::move(dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize));
	m_text = text;
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::UpdateText()
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
	m_renderComponent->SetTexture(std::make_unique<Texture2D>(texture));
	m_needsUpdate = false;
}