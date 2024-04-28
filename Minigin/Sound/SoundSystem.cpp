#include "SoundSystem.h"

#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>

#include "../Render/Resources/ResourceManager.h"
#include "../Render/Resources/SoundFX.h"

namespace dae
{
	class SoundSystem::SoundImpl
	{
		enum class ChannelUse {
			SoundFX,
			Music
		};

	public:
		SoundImpl()
		{
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) throw std::runtime_error(std::string("Failed to initialize SDL Mixer: ") + Mix_GetError());
		}

		~SoundImpl()
		{
			Mix_CloseAudio();
		}

		void LoadSound(int id, const std::string& path)
		{
			m_soundFXs.emplace(id, ResourceManager::GetInstance().LoadSoundFX(path));
		}

		void PlaySoundOnce(int id)
		{
			if (Mix_PlayChannel(-1, m_soundFXs[id]->GetSoundFX(), 0) == -1)
			{
				Mix_FreeChunk(m_soundFXs[id]->GetSoundFX());
				Mix_Quit();
				throw std::runtime_error(std::string("Failed to play soundFX: ") + Mix_GetError());
			}
		}

	private:
		std::unordered_map<int, std::unique_ptr<SoundFX>> m_soundFXs{};
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