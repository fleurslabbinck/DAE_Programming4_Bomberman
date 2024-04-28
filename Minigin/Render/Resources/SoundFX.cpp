#include "SoundFX.h"

#include <SDL_mixer.h>
#include <stdexcept>

namespace dae
{
	Mix_Chunk* SoundFX::GetSoundFX() const {
		return m_soundFX;
	}

	SoundFX::SoundFX(const std::string& fullPath) : m_soundFX(nullptr)
	{
		m_soundFX = Mix_LoadWAV(fullPath.c_str());

		if (m_soundFX == NULL)
		{
			Mix_Quit();
			throw std::runtime_error(std::string("Failed to load soundFX: ") + Mix_GetError());
		}
	}

	SoundFX::~SoundFX()
	{
		Mix_FreeChunk(m_soundFX);
	}
}