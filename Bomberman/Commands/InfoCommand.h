#ifndef INFOCOMMANDS_H
#define INFOCOMMANDS_H

#include "Input/Command.h"

namespace dae
{
	//---------------------------------
	// INFOCOMMAND
	//---------------------------------
	class InfoCommand : public Command
	{
	public:
		InfoCommand() : Command() {}
		void Execute() override;
	private:
		bool m_infoAsked{ false };
	};
}
#endif