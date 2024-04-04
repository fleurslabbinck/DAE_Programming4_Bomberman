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
		MoveCommand(dae::GameObject* gameObject, float speed, glm::vec2 direction) : GameObjectCommand(gameObject), m_speed{ speed }, m_direction{ direction } {}
		void Execute() override;
	protected:
		virtual void MoveObject(const glm::vec2& direction) const;
	private:
		const float m_speed;
		const glm::vec2 m_direction;
	};

	////---------------------------------
	//// MOVELEFT
	////---------------------------------
	//class MoveLeft final : public MoveCommand
	//{
	//public:
	//	MoveLeft(dae::GameObject* gameObject, float speed) : MoveCommand(gameObject, speed) {}
	//	void Execute() override;
	//};
	//
	////---------------------------------
	//// MOVERIGHT
	////---------------------------------
	//class MoveRight final : public MoveCommand
	//{
	//public:
	//	MoveRight(dae::GameObject* gameObject, float speed) : MoveCommand(gameObject, speed) {}
	//	void Execute() override;
	//};
	//
	////---------------------------------
	//// MOVEDOWN
	////---------------------------------
	//class MoveDown final : public MoveCommand
	//{
	//public:
	//	MoveDown(dae::GameObject* gameObject, float speed) : MoveCommand(gameObject, speed) {}
	//	void Execute() override;
	//};
	//
	////---------------------------------
	//// MOVEUP
	////---------------------------------
	//class MoveUp final : public MoveCommand
	//{
	//public:
	//	MoveUp(dae::GameObject* gameObject, float speed) : MoveCommand(gameObject, speed) {}
	//	void Execute() override;
	//};
	//
	////---------------------------------
	//// ATTACKCOMMAND
	////---------------------------------
}
#endif