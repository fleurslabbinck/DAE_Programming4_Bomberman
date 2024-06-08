#include "ToggleMuteCommand.h"

#include <iostream>

#include "ServiceLocator.h"
#include "Sound/LoggingSoundSystem.h"
#include "Sound/NullSoundSystem.h"

namespace dae
{
	//---------------------------------
	// TOGGLEMUTE
	//---------------------------------
	void ToggleMuteCommand::Execute()
	{
		ServiceLocator::GetSoundSystem().ToggleMute();
	}
}