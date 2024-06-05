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
		Notify(static_cast<int>(GameEvent::CONTINUE), nullptr);
	}

	//---------------------------------
	// BACK
	//---------------------------------
	void BackCommand::Execute()
	{
		Notify(static_cast<int>(GameEvent::BACK), nullptr);
	}

	//---------------------------------
	// UP
	//---------------------------------
	void UpCommand::Execute()
	{
		Notify(static_cast<int>(GameEvent::UP), nullptr);
	}
	//---------------------------------
	// DOWN
	//---------------------------------
	void DownCommand::Execute()
	{
		Notify(static_cast<int>(GameEvent::DOWN), nullptr);
	}
}