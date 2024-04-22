#ifndef BOMBCOMMAND_H
#define BOMBCOMMAND_H

#include "../minigin/Input/Command.h"

namespace dae
{
	//---------------------------------
	// BOMBCOMMAND
	//---------------------------------
	class BombCommand : public GameObjectCommand
	{
	public:
		BombCommand(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
		void Execute() override;
	};
}
#endif