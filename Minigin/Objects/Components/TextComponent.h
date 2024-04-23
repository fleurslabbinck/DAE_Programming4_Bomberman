#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "RenderComponent.h"
#include "../../Render/Resources/Font.h"
#include "../../Render/Resources/Texture2D.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//TEXTCOMPONENT
	//---------------------------------
	class TextComponent : public BaseComponent
	{
	public:
		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void SetText(const std::string& text);
		void UpdateText();

		bool NeedsUpdate() const { return m_needsUpdate; };
		RenderComponent* GetRenderComponent() const { return m_renderComponent.get(); }

		explicit TextComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text);
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
	private:
		std::unique_ptr<RenderComponent> m_renderComponent{ nullptr };

		bool m_needsUpdate{ true };
		std::string m_text;
		std::unique_ptr<Font> m_font{};

		SDL_Texture* GetTexture();
	};
}
#endif