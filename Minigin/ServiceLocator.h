#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <vector>
#include <memory>

#include "Sound/SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static void RegisterSoundSystem()
		{
			m_soundSystem = std::make_unique<SoundSystem>();
		}

		static SoundSystem& GetSoundSystem()
		{
			return *m_soundSystem.get();
		}

	private:
		inline static std::unique_ptr<SoundSystem> m_soundSystem{};
	};
}
#endif