#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "Components/RenderComponent.h"
#include "Resources/Font.h"
#include "Resources/Texture2D.h"

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

		void SetText(const std::string& text);
		void UpdateText();
		SDL_Texture* GetTexture();
		bool NeedsUpdate() const { return m_needsUpdate; };

		explicit TextComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text);
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
	protected:
		std::unique_ptr<RenderComponent> m_renderComponent{ nullptr };

		bool m_needsUpdate{ true };
		std::string m_text;
		std::unique_ptr<Font> m_font{};
	};
}
#endif