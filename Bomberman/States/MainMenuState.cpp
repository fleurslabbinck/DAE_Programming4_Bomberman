#include "MainMenuState.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"
#include "PlayState.h"

namespace dae
{
	GameState* MainMenuState::HandleGame(Input input)
	{
		if (input == static_cast<int>(gameInput::GameInput::Continue)) return new PlayState();

		return nullptr;
	}

	void MainMenuState::OnEnter() const
	{
		BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Menu));
	}
}