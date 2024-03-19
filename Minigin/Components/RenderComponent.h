#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "BaseComponent.h"
#include "Resources/Texture2D.h"

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

		void SetTexture(const std::string& filename);
		void SetTexture(std::unique_ptr<Texture2D> texture);
		const Texture2D* GetTexture() const { return m_texture.get(); }

		explicit RenderComponent(GameObject* pOwner) : BaseComponent(pOwner) {}
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;

	private:
		std::unique_ptr<Texture2D> m_texture{};
	};
}
#endif