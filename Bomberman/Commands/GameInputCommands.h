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

	private:
		bool m_pressed{ false };
	};

	//---------------------------------
	// BACKCOMMAND
	//---------------------------------
	class BackCommand : public Command, public Subject
	{
	public:
		BackCommand() : Command() {}
		void Execute() override;

	private:
		bool m_pressed{ false };
	};
}
#endif