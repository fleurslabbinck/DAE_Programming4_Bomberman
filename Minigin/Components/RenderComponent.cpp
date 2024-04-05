#include "Components/RenderComponent.h"
#include "Render/ResourceManager.h"
#include "Render/Renderer.h"

namespace dae
{
	//---------------------------------
	//RENDERCOMPONENT
	//---------------------------------
	void RenderComponent::Render(const glm::vec2& pos) const
	{
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

	glm::vec2 RenderComponent::GetDimensions() const
	{
		int textureWidth, textureHeight;
		SDL_QueryTexture(GetTexture()->GetSDLTexture(), NULL, NULL, &textureWidth, &textureHeight);

		return { static_cast<float>(textureWidth), static_cast<float>(textureHeight) };
	}

	void RenderComponent::SetTexture(const std::string& filename)
	{
		m_texture = std::move(ResourceManager::GetInstance().LoadTexture(filename));
	}

	void RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
	{
		m_texture = std::move(texture);
	}
}