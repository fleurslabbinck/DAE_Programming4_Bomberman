#ifndef GAMEINPUTCOMMANDS_H
#define GAMEINPUTCOMMANDS_H

#include "Input/Command.h"
#include "Observers/Subject.h"

namespace dae
{
	//---------------------------------
	// CONTINUECOMMAND
	//---------------------------------
	class ContinueCommand : public Command, public Subject
	{
	public:
		ContinueCommand() : Command() {}
		void Execute() override;
	};

	//---------------------------------
	// BACKCOMMAND
	//---------------------------------
	class BackCommand : public Command, public Subject
	{
	public:
		BackCommand() : Command() {}
		void Execute() override;
	};

	//---------------------------------
	// UPCOMMAND
	//---------------------------------
	class UpCommand : public Command, public Subject
	{
	public:
		UpCommand() : Command() {}
		void Execute() override;
	};

	//---------------------------------
	// DOWNCOMMAND
	//---------------------------------
	class DownCommand : public Command, public Subject
	{
	public:
		DownCommand() : Command() {}
		void Execute() override;
	};
}
#endif