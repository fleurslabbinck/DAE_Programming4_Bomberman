#ifndef LOGGINGSOUNDSYSTEM_H
#define LOGGINGSOUNDSYSTEM_H

#include "SoundSystem.h"

namespace dae
{
	//---------------------------------
	//LOGGING SOUND SYSTEM
	//---------------------------------
	class LoggingSoundSystem final : public BaseSoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem);

		void LoadSoundFX(int id, const std::string& path) override;
		void PlaySoundFX(int id) override;
		void UpdateSoundFX() override;

	private:
		std::unique_ptr<SoundSystem> m_SoundSystemPtr{};
	};
}

#endif