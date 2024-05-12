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

		GameState* HandleGame(Input input) override;
		void OnEnter() const override;
	};
}
#endif