#ifndef SOUNDFX_H
#define SOUNDFX_H

#include <string>

struct Mix_Chunk;
namespace dae
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class SoundFX final
	{
	public:
		Mix_Chunk* GetSoundFX() const;
		explicit SoundFX(const std::string& fullPath);
		~SoundFX();

		SoundFX(const SoundFX&) = delete;
		SoundFX(SoundFX&&) = delete;
		SoundFX& operator= (const SoundFX&) = delete;
		SoundFX& operator= (const SoundFX&&) = delete;
	private:
		Mix_Chunk* m_soundFX;
	};
}
#endif