#include "SkipLevelCommand.h"

#include "../BombermanUtil.h"

namespace dae
{
	//---------------------------------
	// SKIPLEVEL
	//---------------------------------
	void SkipLevelCommand::Execute()
	{
		Notify(static_cast<int>(GameEvent::STAGE_WON), nullptr);
	}
}