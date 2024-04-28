#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <vector>
#include <memory>

#include "Sound/NullSoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static void RegisterSoundSystem(std::unique_ptr<BaseSoundSystem> soundSystem)
		{
			m_soundSystemPtr = std::move(soundSystem);
		}

		static BaseSoundSystem& GetSoundSystem()
		{
			if (!m_soundSystemPtr) return *m_nullSoundSystemPtr.get();

			return *m_soundSystemPtr.get();
		}

	private:
		inline static std::unique_ptr<BaseSoundSystem> m_soundSystemPtr{ nullptr };
		inline static std::unique_ptr<NullSoundSystem> m_nullSoundSystemPtr{ std::make_unique<NullSoundSystem>() };
	};
}
#endif