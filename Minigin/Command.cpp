#pragma once
#include "Command.h"

//---------------------------------
// MOVELEFT
//---------------------------------
void MoveLeft::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };

	if (!gameObject) return;
	
	gameObject->Move(dae::MoveDirection::Left);
}

//---------------------------------
// MOVERIGHT
//---------------------------------
void MoveRight::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };

	if (!gameObject) return;

	gameObject->Move(dae::MoveDirection::Right);
}

//---------------------------------
// MOVEDOWN
//---------------------------------
void MoveDown::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };

	if (!gameObject) return;

	gameObject->Move(dae::MoveDirection::Down);
}

//---------------------------------
// MOVEUP
//---------------------------------
void MoveUp::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };

	if (!gameObject) return;

	gameObject->Move(dae::MoveDirection::Up);
}