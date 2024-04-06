#include "Components/RenderComponent.h"
#include "Render/ResourceManager.h"
#include "Render/Renderer.h"

namespace dae
{
	//---------------------------------
	//RENDERCOMPONENT
	//---------------------------------
	RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
		: BaseComponent(pOwner)
	{
		m_texture = std::move(ResourceManager::GetInstance().LoadTexture(filename));
	}

	RenderComponent::RenderComponent(GameObject* pOwner, std::unique_ptr<Texture2D>&& texture)
		: BaseComponent(pOwner)
	{
		m_texture = std::move(texture);
	}

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

	void RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
	{
		m_texture = std::move(texture);
	}
}