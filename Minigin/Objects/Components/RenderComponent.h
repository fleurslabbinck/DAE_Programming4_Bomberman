#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "../../Render/Resources/Texture2D.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//RENDERCOMPONENT
	//---------------------------------
	class RenderComponent : public BaseComponent
	{
	public:
		void Render(const glm::vec2& pos) const override;

		void SetTexture(std::unique_ptr<Texture2D> texture);
		const Texture2D* GetTexture() const { return m_texture.get(); }

		explicit RenderComponent(GameObject* pOwner, const std::string& filename);
		explicit RenderComponent(GameObject* pOwner, std::unique_ptr<Texture2D>&& texture);
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;

	private:
		std::unique_ptr<Texture2D> m_texture{};
	};
}
#endif