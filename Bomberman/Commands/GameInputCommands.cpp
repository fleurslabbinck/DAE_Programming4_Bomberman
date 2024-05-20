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
		if (m_pressed) return;
		Notify(static_cast<int>(GameEvent::CONTINUE), nullptr);
		m_pressed = true;
	}

	//---------------------------------
	// BACK
	//---------------------------------
	void BackCommand::Execute()
	{
		if (m_pressed) return;
		Notify(static_cast<int>(GameEvent::BACK), nullptr);
		m_pressed = true;
	}
}