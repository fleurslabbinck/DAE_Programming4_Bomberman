#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <memory>
#include <string>

namespace dae
{
	class SoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();
		
		virtual void LoadSound(int id, const std::string& path);
		virtual void PlaySoundOnce(int id);

	private:

		class SoundImpl;
		std::unique_ptr<SoundImpl> m_pImpl;
	};
}
#endif