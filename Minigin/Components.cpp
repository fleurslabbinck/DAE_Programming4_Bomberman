#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

//---------------------------------
//BASE COMPONENT
//---------------------------------
void dae::Component::FixedUpdate(float) {}

void dae::Component::Update(float) {}

void dae::Component::Render(const Transform&) const {}

void dae::Component::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::Component::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

//---------------------------------
//BITMAPCOMPONENT
//---------------------------------
void dae::BitmapComponent::Render(const Transform& transform) const
{
	const auto& pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x + m_transform.GetPosition().x, pos.y + m_transform.GetPosition().y);
}

//---------------------------------
//TEXTCOMPONENT
//---------------------------------
//dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font)
//	: m_needsUpdate(true), m_text(text), m_font(std::move(font))
//{
//	m_texture = nullptr;
//}

void dae::TextComponent::Update(float)
{
	if (m_needsUpdate)
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
		m_texture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render(const Transform&) const
{
	if (m_texture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::TextComponent::Initialize(const std::string& text, std::shared_ptr<Font> font)
{
	m_needsUpdate = true;
	m_text = text;
	m_font = std::move(font);
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}