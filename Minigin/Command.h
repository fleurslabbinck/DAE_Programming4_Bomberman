#pragma once
#include "GameObject.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
}

class GameActorCommand : public Command
{
	GameObject* m_GameObject;
};

class Jump final : public Command
{
public:
	void Execute() override { Jump(); }
};