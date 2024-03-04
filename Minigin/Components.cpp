#include <stdexcept>
#include <SDL_ttf.h>
#include <iomanip>
#include <sstream>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "TimeManager.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

//---------------------------------
//BASE COMPONENT
//---------------------------------
void dae::BaseComponent::FixedUpdate() { for (auto& component : m_subComponents) component->FixedUpdate(); }

void dae::BaseComponent::Update() { for (auto& component : m_subComponents) component->Update(); }

void dae::BaseComponent::Render(const glm::vec3& pos) const { for (auto& component : m_subComponents) component->Render(pos); }

//---------------------------------
//TRANSFORMCOMPONENT
//---------------------------------
void dae::TransformComponent::UpdateWorldPosition()
{
	if (GetOwner()->GetParent() == nullptr) m_worldPosition = m_localPosition;
	else m_worldPosition = GetOwner()->GetParent()->GetTransform()->GetWorldPosition() + m_localPosition;

	m_positionIsDirty = false;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

void dae::TransformComponent::SetPositionDirty()
{
	m_positionIsDirty = true;

	GameObject* owner{ GetOwner() };

	for (int idx{}; idx < owner->GetChildCount(); ++idx)
	{
		owner->GetChildAt(idx)->GetTransform()->SetPositionDirty();
	}

}

glm::vec3 dae::TransformComponent::GetWorldPosition()
{
	if (m_positionIsDirty) UpdateWorldPosition();

	return m_worldPosition;
}

//---------------------------------
//ROTATORCOMPONENT
//---------------------------------
void dae::RotatorComponent::Update()
{
	dae::TransformComponent* transform = GetOwner()->GetTransform();

	const glm::vec3 localPos{transform->GetLocalPosition() };

	const glm::vec3 rotatedPos{ RotatePoint(localPos) };

	transform->SetLocalPosition(rotatedPos);
}

glm::vec3 dae::RotatorComponent::RotatePoint(const glm::vec3& pos) const
{
	const float radians = glm::radians(m_rotation);

	const glm::mat3 rotationMatrix = glm::mat3(
			glm::vec3(cos(radians), -sin(radians), 0.0f),
			glm::vec3(sin(radians), cos(radians), 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));

	return rotationMatrix * pos;
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
	m_texture = std::move(ResourceManager::GetInstance().LoadTexture(filename));
}

void dae::RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
{
	m_texture = std::move(texture);
}

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

void dae::FPSComponent::Initialize(const std::string& fontPath, int fontSize, const std::string& text)
{
	m_textComponent->Initialize(fontPath, fontSize, text);
}