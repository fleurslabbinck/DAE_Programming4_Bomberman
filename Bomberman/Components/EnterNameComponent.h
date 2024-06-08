#ifndef ENTERNAMECOMPONENT_H
#define ENTERNAMECOMPONENT_H

#include <SDL.h>
#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"

#include "../BombermanUtil.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	//---------------------------------
	//ENTERNAMECOMPONENT
	//---------------------------------
	class EnterNameComponent : public BaseComponent, public Subject, public Observer
	{
	public:
		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void OnNotify(Event event, GameObject* gameObject) override;

		explicit EnterNameComponent(GameObject* pOwner, const std::string& fontpath, int fontSize, SDL_Color textColor, SDL_Color shadowColor);
		EnterNameComponent(const TextComponent& other) = delete;
		EnterNameComponent(TextComponent&& other) = delete;
	private:
		glm::vec2 m_enterNamePos{};
		glm::vec2 m_playerNamePos{};
		std::string m_playerInput{};
		std::string m_playerNameUpper{};
		std::unique_ptr<TextComponent> m_enterNameText;
		std::unique_ptr<TextComponent> m_playerName;
	};
}
#endif