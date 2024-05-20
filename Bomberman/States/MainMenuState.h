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
		std::unique_ptr<GameState> HandleGame() override { return std::move(m_state); }
		void OnEnter() const override;
	};
}
#endif