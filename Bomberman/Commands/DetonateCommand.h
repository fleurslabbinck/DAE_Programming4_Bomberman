#ifndef DETONATECOMMAND_H
#define DETONATECOMMAND_H

#include "Input/Command.h"

namespace dae
{
	//---------------------------------
	// DETONATECOMMAND
	//---------------------------------
	class DetonateCommand : public GameObjectCommand
	{
	public:
		DetonateCommand(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
		void Execute() override;
	};
}
#endif