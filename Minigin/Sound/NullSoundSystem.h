#ifndef NULLSOUNDSYSTEM_H
#define NULLSOUNDSYSTEM_H

#include "BaseSoundSystem.h"

namespace dae
{
	//---------------------------------
	//NULL SOUND SYSTEM
	//---------------------------------
	class NullSoundSystem : public BaseSoundSystem
	{
	public:
		NullSoundSystem() = default;

		void LoadSoundFX(int, const std::string&) override {}
		void PlaySoundFX(int, int) override {}
		void UpdateSoundFX() override {}
		void ToggleMute() override {}
		void SignalEnd() override {}
	};
}
#endif