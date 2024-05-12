#ifndef GAMEINPUTCOMMANDS_H
#define GAMEINPUTCOMMANDS_H

#include "Input/Command.h"

namespace dae
{
	//---------------------------------
	// CONTINUECOMMAND
	//---------------------------------
	class ContinueCommand : public Command
	{
	public:
		ContinueCommand() : Command() {}
		void Execute() override;
	};

	//---------------------------------
	// BACKCOMMAND
	//---------------------------------
	class BackCommand : public Command
	{
	public:
		BackCommand() : Command() {}
		void Execute() override;
	};
}
#endif