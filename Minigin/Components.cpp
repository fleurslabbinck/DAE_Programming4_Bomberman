#include <stdexcept>
#include <SDL_ttf.h>
#include <iomanip>
#include <sstream>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TimeManager.h"

//---------------------------------
//BASE COMPONENT
//---------------------------------
void dae::BaseComponent::Update() { for (auto component : m_subComponents) component->Update(); }

void dae::BaseComponent::Render(const glm::vec3& pos) const { for (auto component : m_subComponents) component->Render(pos); }

//---------------------------------
//TRANSFORMCOMPONENT
//---------------------------------
void dae::TransformComponent::Update()
{

}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


//---------------------------------
//RENDERCOMPONENT
//---------------------------------
void dae::RenderComponent::Render(const glm::vec3& pos) const
{
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_texture = texture;
}

//---------------------------------
//TEXTCOMPONENT
//---------------------------------
void dae::TextComponent::Update()
{
	if (m_needsUpdate) UpdateText();
}

void dae::TextComponent::Initialize(std::shared_ptr<Font> font, const std::string& text)
{
	m_font = font;
	m_text = text;
}

// This implementation uses the "dirty flag" pattern
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
	m_renderComponent->SetTexture(std::make_shared<Texture2D>(texture));
	m_needsUpdate = false;
}

//---------------------------------
//FPSCOMPONENT
//---------------------------------

void dae::FPSComponent::Update()
{
	constexpr float maxTime{ 0.1f };

	TimeManager& time = TimeManager::GetInstance();
	
	++m_frameCount;
	m_accumulatedTime += time.GetDeltaTime();

	if (m_accumulatedTime >= maxTime)
	{
		m_textComponent->SetText(std::format("{:.1f}", m_frameCount / m_accumulatedTime));

		m_frameCount = 0;
		m_accumulatedTime = 0;
	}

	if (m_textComponent->NeedsUpdate()) m_textComponent->UpdateText();
}

void dae::FPSComponent::Initialize(std::shared_ptr<Font> font, const std::string& text)
{
	m_textComponent->Initialize(font, text);
}