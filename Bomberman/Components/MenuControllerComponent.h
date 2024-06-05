#ifndef CONTROLLERCOMPONENT_H
#define CONTROLLERCOMPONENT_H

#include <SDL.h>
#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

#include "../BombermanUtil.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class RenderComponent;

	//---------------------------------
	//MENUCONTROLLERCOMPONENT
	//---------------------------------
	class MenuControllerComponent : public BaseComponent, public Subject, public Observer
	{
	public:
		struct MenuItem {
			GameEvent event{};
			glm::vec2 dimensions{};
			glm::vec2 startPos{};
			std::unique_ptr<TextComponent> textComp{ nullptr };
		};

		struct MenuPointer {
			uint8_t menuIdx{};
			const float offset{ 10.f };
			MenuItem* currentMenuItem{ nullptr };
			std::unique_ptr<RenderComponent> renderComp{ nullptr };
		};

		void Render(const glm::vec2& pos) const override;

		void OnNotify(Event event, GameObject* gameObject) override;

		explicit MenuControllerComponent(GameObject* pOwner, const std::string& fontpath, int fontSize);
		MenuControllerComponent(const TextComponent& other) = delete;
		MenuControllerComponent(TextComponent&& other) = delete;
	private:
		SDL_Color m_textColor{ 255, 255, 255, 255 };
		SDL_Color m_shadowColor{ 200, 200, 200, 200 };

		glm::vec2 m_creditPos{};
		std::unique_ptr<TextComponent> m_credits;
		
		MenuPointer m_menuPointer{};
		std::vector<MenuItem> m_menuItems{};
	};
}
#endif