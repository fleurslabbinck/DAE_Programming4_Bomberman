#include "SoundSystem.h"

#include <SDL.h>
#include <SDL_main.h>
#include <../SDL2_mixer/include/SDL_mixer.h>
#include <../SDL2_wavpack/include/wavpack.h>
#include <stdexcept>
#include <unordered_map>

namespace dae
{
	class SoundSystem::SoundImpl
	{
	public:
		SoundImpl()
		{
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) throw std::runtime_error("Failed to initialize SDL Mixer\n");
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
				throw std::runtime_error("Failed to load sound\n");
			}

			m_sounds.emplace(id, sound);
		}

		void PlaySoundOnce(int id)
		{
			if (Mix_PlayChannel(-1, m_sounds[id], 0) == -1)
			{
				Mix_FreeChunk(m_sounds[id]);
				throw std::runtime_error("Failed to play sound\n");
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