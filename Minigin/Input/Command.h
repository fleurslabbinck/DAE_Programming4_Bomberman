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
		Move(dae::GameObject* gameObject, float speed) : GameObjectCommand(gameObject), m_speed{ speed } {}
		void Execute() override;

		void SetSpeed(float speed) { m_speed = speed; }
		void SetDirection(const glm::vec2& dir);
		void Reset() { m_set = false; }

		bool GetSet() const { return m_set; }

	private:
		float m_speed{};
		glm::vec2 m_direction{};
		bool m_set{ false };
		bool m_setY{ false };
	};
}
#endif