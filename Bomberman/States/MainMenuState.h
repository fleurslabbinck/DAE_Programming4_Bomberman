#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Game/GameState.h"

namespace dae
{
	class MainMenuState final : public GameState
	{
	public:
		MainMenuState() = default;
		MainMenuState(const MainMenuState& other) = delete;
		MainMenuState(MainMenuState&& other) = delete;
		MainMenuState& operator=(const MainMenuState& other) = delete;
		MainMenuState& operator=(MainMenuState&& other) = delete;

		void OnNotify(Event event, GameObject* gameObject) override;
		void OnEnter() const override;

	private:
		uint8_t m_singlePlayer{ 1 };
		uint8_t m_multiPlayer{ 2 };
	};
}
#endif