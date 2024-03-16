#pragma once
#include "Command.h"

//---------------------------------
// MOVECOMMAND
//---------------------------------
void Move::Execute()
{
	GetGameObject()->Move();
}