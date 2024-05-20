#ifndef BASESOUNDSYSTEM_H
#define BASESOUNDSYSTEM_H

#include <string>

namespace dae
{
	struct PlayMessage {
		int id;
		int volume;
	};

	//---------------------------------
	//BASE SOUND SYSTEM
	//---------------------------------
	class BaseSoundSystem
	{
	public:
		BaseSoundSystem() = default;
		virtual ~BaseSoundSystem() = default;
		BaseSoundSystem(const BaseSoundSystem& other) = delete;
		BaseSoundSystem(BaseSoundSystem&& other) = delete;
		virtual BaseSoundSystem& operator=(const BaseSoundSystem& other) = delete;
		virtual BaseSoundSystem& operator=(BaseSoundSystem&& other) = delete;

		virtual void LoadSoundFX(int id, const std::string& path) = 0;
		virtual void PlaySoundFX(int id, int volume) = 0;
		virtual void UpdateSoundFX() = 0;
		virtual void SignalEnd() = 0;
	};
}

#endif