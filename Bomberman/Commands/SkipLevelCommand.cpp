#include "SkipLevelCommand.h"

#include "../BombermanManager.h"
#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	// SKIPLEVEL
	//---------------------------------
	void SkipLevelCommand::Execute()
	{
		BombermanManager::GetInstance().AddToScore(600);
		Notify(static_cast<int>(GameEvent::STAGE_WON), nullptr);
	}
}