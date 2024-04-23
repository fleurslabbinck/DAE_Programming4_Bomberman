#ifndef COMMAND_H
#define COMMAND_H

#include "../Objects/GameObject.h"

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
}
#endif