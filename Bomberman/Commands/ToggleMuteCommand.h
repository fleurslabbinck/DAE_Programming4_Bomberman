#ifndef TOGGLEMUTECOMMAND_H
#define TOGGLEMUTECOMMAND_H

#include "Input/Command.h"
#include "Observers/Subject.h"

namespace dae
{
	//---------------------------------
	// TOGGLEMUTECOMMAND
	//---------------------------------
	class ToggleMuteCommand : public Command, public Subject
	{
	public:
		ToggleMuteCommand() : Command() {}
		void Execute() override;
	};
}
#endif