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

	private:
		const float m_speed;
		const glm::vec2 m_direction;
		static glm::vec2 m_lastDirection;
		glm::vec2 m_targetPos{};
		const float m_targetOffset{ 0.5f };
	};

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