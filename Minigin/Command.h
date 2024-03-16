#pragma once
#include "GameObject.h"

//---------------------------------
// COMMAND
//---------------------------------
class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};


//---------------------------------
// GAMEOBJECTCOMMAND
//---------------------------------
class GameObjectCommand : public Command
{
	dae::GameObject* m_GameObject;

public:
	GameObjectCommand(dae::GameObject* gameObject) : m_GameObject{ gameObject } {};
	virtual ~GameObjectCommand() = default;

protected:
	dae::GameObject* GetGameObject() const { return m_GameObject; }
};


//---------------------------------
// MOVECOMMAND
//---------------------------------
class Move final : public GameObjectCommand
{
public:
	void Execute() override;
};