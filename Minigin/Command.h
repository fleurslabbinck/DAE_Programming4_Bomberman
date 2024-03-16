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

protected:
	dae::GameObject* GetGameObject() const { return m_GameObject; }
};

//---------------------------------
// MOVE
//---------------------------------
class Move final : public GameObjectCommand
{
public:
	Move() = default;
	Move(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	void Execute() override;

	void SetDirection(const glm::vec3& dir);
	void Reset() { m_set = false; }

	bool GetSet() const { return m_set; }

private:
	glm::vec3 m_direction{};
	bool m_set{ false };
	bool m_setY{ false };
};