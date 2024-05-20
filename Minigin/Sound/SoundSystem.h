#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <memory>

#include "BaseSoundSystem.h"

namespace dae
{
	//---------------------------------
	//SDL SOUND SYSTEM
	//---------------------------------
	class SoundSystem final : public BaseSoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();
		
		void LoadSoundFX(int id, const std::string& path) override;
		void PlaySoundFX(int id, int volume) override;
		void UpdateSoundFX() override;
		void SignalEnd() override;

	private:
		class SoundImpl;
		std::unique_ptr<SoundImpl> m_pImpl;
	};
}
#endif