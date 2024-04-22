#ifndef	HUDCOMPONENT_H
#define	HUDCOMPONENT_H

#include <vector>
#include "../Minigin/Objects/Components/BaseComponent.h"
#include "Observers/Observer.h"

namespace dae
{
	class TextComponent;

	class HUDComponent final : public BaseComponent, public Observer
	{
	public:
		struct HUDMessage {
			int value;
			std::string text;
			glm::vec2 position;
			std::unique_ptr<TextComponent> textComponent;

			void SetMessage();
		};

		void OnNotify(GameEvent event, GameObject* gameObject);

		void Render(const glm::vec2& pos) const;

		explicit HUDComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text = " ");
		HUDComponent(const HUDComponent& other) = delete;
		HUDComponent(HUDComponent&& other) = delete;

	private:
		HUDMessage m_livesMessage{};
		HUDMessage m_scoreMessage{};

		int GetLives(GameObject* gameObject) const;
		int GetScore(GameObject* gameObject) const;
	};
}
#endif