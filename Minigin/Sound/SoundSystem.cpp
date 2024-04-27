#include "SoundSystem.h"

#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>

namespace dae
{
	class SoundSystem::SoundImpl
	{
	public:
		SoundImpl()
		{
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) std::cout << "Failed to initialize SDL Mixer! " << Mix_GetError() << std::endl;
		}

		~SoundImpl()
		{
			for (auto& sound : m_sounds) Mix_FreeChunk(sound.second);

			Mix_Quit();
		}

		void LoadSound(int id, const std::string& path)
		{
			Mix_Chunk* sound{ Mix_LoadWAV(path.c_str()) };

			if (sound == NULL)
			{
				std::cout << "Failed to load sound! " << Mix_GetError() << std::endl;
				Mix_Quit();
			}

			m_sounds.emplace(id, sound);
		}

		void PlaySoundOnce(int id)
		{
			if (Mix_PlayChannel(-1, m_sounds[id], 0) == -1)
			{
				std::cout << "Failed to play sound! " << Mix_GetError() << std::endl;
				Mix_FreeChunk(m_sounds[id]);
				Mix_Quit();
			}

			Mix_PlayChannel(-1, m_sounds[id], 0);
		}

	private:
		std::unordered_map<int, Mix_Chunk*> m_sounds{};
	};

	SoundSystem::SoundSystem()
		: m_pImpl{ std::make_unique<SoundImpl>() }
	{

	}

	SoundSystem::~SoundSystem() = default;

	void SoundSystem::LoadSound(int id, const std::string& path)
	{
		m_pImpl->LoadSound(id, path);
	}

	void SoundSystem::PlaySoundOnce(int id)
	{
		m_pImpl->PlaySoundOnce(id);
	}
}