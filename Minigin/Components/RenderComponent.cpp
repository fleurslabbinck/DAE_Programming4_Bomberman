#include "Components/RenderComponent.h"
#include "Render/Resources/ResourceManager.h"
#include "Render/Renderer.h"
#include "BombermanUtil.h"

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
		const glm::vec2 dimensions{ m_texture->GetSize() };
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, dimensions.x * constants::WINDOW_SCALE, dimensions.y * constants::WINDOW_SCALE);
	}

	void RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
	{
		m_texture = std::move(texture);
	}
}