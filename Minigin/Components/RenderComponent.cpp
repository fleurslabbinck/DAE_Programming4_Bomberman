#include "Components/RenderComponent.h"
#include "Singletons/ResourceManager.h"
#include "Singletons/Renderer.h"

//---------------------------------
//RENDERCOMPONENT
//---------------------------------
void dae::RenderComponent::Render(const glm::vec2& pos) const
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
