#include "GameInputCommands.h"

#include "../BombermanUtil.h"
#include "../BombermanManager.h"

namespace dae
{
	//---------------------------------
	// CONTINUE
	//---------------------------------
	void ContinueCommand::Execute()
	{
		BombermanManager::GetInstance().HandleGame(static_cast<int>(gameInput::GameInput::Continue));
	}

	//---------------------------------
	// BACK
	//---------------------------------
	void BackCommand::Execute()
	{
		BombermanManager::GetInstance().HandleGame(static_cast<int>(gameInput::GameInput::Back));
	}
}