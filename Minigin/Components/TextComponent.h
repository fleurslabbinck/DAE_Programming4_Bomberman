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

		void Initialize(const std::string& fontPath, int fontSize, const std::string& text);
		void SetText(const std::string& text);
		void UpdateText();
		bool NeedsUpdate() const { return m_needsUpdate; };

		explicit TextComponent(GameObject* pOwner) : BaseComponent(pOwner), m_renderComponent{ std::make_unique<RenderComponent>(pOwner) } { m_subComponents.push_back(m_renderComponent.get()); }
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
	protected:
		std::unique_ptr<RenderComponent> m_renderComponent;

		bool m_needsUpdate{ true };
		std::string m_text{};
		std::unique_ptr<Font> m_font{ nullptr };
	};
}
#endif