#ifndef COMMAND_H
#define COMMAND_H

#include "GameObject.h"

namespace dae
{
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
		dae::GameObject* m_gameObject{ nullptr };

	public:
		GameObjectCommand() = default;
		GameObjectCommand(dae::GameObject* gameObject) : m_gameObject{ gameObject } {};
		virtual ~GameObjectCommand() = default;

	protected:
		dae::GameObject* GetGameObject() const { return m_gameObject; }
	};


	//---------------------------------
	// MOVECOMMAND
	//---------------------------------
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(dae::GameObject* gameObject) : GameObjectCommand(gameObject) {}
	protected:
		virtual float CalculateOffset() const;
	private:
		const float m_speed{ 150.f };
	};

	//---------------------------------
	// MOVELEFT
	//---------------------------------
	class MoveLeft final : public MoveCommand
	{
	public:
		MoveLeft(dae::GameObject* gameObject) : MoveCommand(gameObject) {}
		void Execute() override;
	};

	//---------------------------------
	// MOVERIGHT
	//---------------------------------
	class MoveRight final : public MoveCommand
	{
	public:
		MoveRight(dae::GameObject* gameObject) : MoveCommand(gameObject) {}
		void Execute() override;
	};

	//---------------------------------
	// MOVEDOWN
	//---------------------------------
	class MoveDown final : public MoveCommand
	{
	public:
		MoveDown(dae::GameObject* gameObject) : MoveCommand(gameObject) {}
		void Execute() override;
	};

	//---------------------------------
	// MOVEUP
	//---------------------------------
	class MoveUp final : public MoveCommand
	{
	public:
		MoveUp(dae::GameObject* gameObject) : MoveCommand(gameObject) {}
		void Execute() override;
	};
}
#endif