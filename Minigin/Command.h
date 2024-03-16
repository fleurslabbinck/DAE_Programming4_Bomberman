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
	dae::GameObject* m_GameObject{ nullptr };

public:
	GameObjectCommand() = default;
	GameObjectCommand(dae::GameObject* gameObject) : m_GameObject{ gameObject } {};
	virtual ~GameObjectCommand() = default;

	void Enable() { m_enabled = true; }
	void Disable() { m_enabled = false; }
	bool IsEnabled() const { return m_enabled; }

protected:
	dae::GameObject* GetGameObject() const { return m_GameObject; }
	bool m_enabled{ false };
};


//---------------------------------
// MOVELEFT
//---------------------------------
class MoveLeft final : public GameObjectCommand
{
public:
	MoveLeft() = default;
	MoveLeft(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	void Execute() override;
};

//---------------------------------
// MOVERIGHT
//---------------------------------
class MoveRight final : public GameObjectCommand
{
public:
	MoveRight() = default;
	MoveRight(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	void Execute() override;
};

//---------------------------------
// MOVEDOWN
//---------------------------------
class MoveDown final : public GameObjectCommand
{
public:
	MoveDown() = default;
	MoveDown(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	void Execute() override;
};

//---------------------------------
// MOVEUP
//---------------------------------
class MoveUp final : public GameObjectCommand
{
public:
	MoveUp() = default;
	MoveUp(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	void Execute() override;
};