#include "LoggingSoundSystem.h"

#include <iostream>

namespace dae
{
	LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		: m_SoundSystemPtr{ std::move(soundSystem) }
	{

	}

	void LoggingSoundSystem::LoadSoundFX(int id, const std::string& path)
	{
		m_SoundSystemPtr->LoadSoundFX(id, path);
		std::cout << "Loading " << id << " from path: " << path << std::endl;
	}

	void LoggingSoundSystem::PlaySoundFX(int id)
	{
		m_SoundSystemPtr->PlaySoundFX(id);
		std::cout << "Added " << id << " to the queue\n";
	}

	void LoggingSoundSystem::UpdateSoundFX()
	{
		if (!m_SoundSystemPtr->HasPendingMessages()) return;

		m_SoundSystemPtr->UpdateSoundFX();
		std::cout << "Updating queue\n";
	}
}