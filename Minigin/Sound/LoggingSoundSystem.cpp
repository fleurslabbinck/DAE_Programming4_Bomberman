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

	void LoggingSoundSystem::PlaySoundFX(int id, int volume)
	{
		m_SoundSystemPtr->PlaySoundFX(id, volume);
		std::cout << "Added " << id << " to the queue\n";
	}

	void LoggingSoundSystem::UpdateSoundFX()
	{
		m_SoundSystemPtr->UpdateSoundFX();
		std::cout << "Updating queue\n";
	}

	void LoggingSoundSystem::ToggleMute()
	{
		m_SoundSystemPtr->ToggleMute();
		std::cout << "Toggled mute\n";
	}

	void LoggingSoundSystem::SignalEnd()
	{
		m_SoundSystemPtr->SignalEnd();
		std::cout << "Stop thread\n";
	}
}