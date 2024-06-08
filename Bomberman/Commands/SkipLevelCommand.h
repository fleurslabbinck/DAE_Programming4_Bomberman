#ifndef SKIPLEVELCOMMAND_H
#define SKIPLEVELCOMMAND_H

#include "Input/Command.h"
#include "Observers/Subject.h"

namespace dae
{
	//---------------------------------
	// SKIPLEVELCOMMAND
	//---------------------------------
	class SkipLevelCommand : public Command, public Subject
	{
	public:
		SkipLevelCommand() : Command() {}
		void Execute() override;
	};
}
#endif