#include "SoundSystem.h"

#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>
#include <future>
#include <mutex>

#include "../Render/Resources/ResourceManager.h"
#include "../Render/Resources/SoundFX.h"
#include "../../Bomberman/BombermanUtil.h"

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
			: m_signalPromise{}
			, m_signalEnd{ m_signalPromise.get_future() }
		{
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) throw std::runtime_error(std::string("Failed to initialize SDL Mixer: ") + Mix_GetError());

			for (auto& message : m_pendingMessages) message.id = static_cast<int>(sound::SoundId::Empty);
		}

		~SoundImpl()
		{
			Mix_CloseAudio();
		}

		void LoadSoundFX(int id, const std::string& path)
		{
			m_soundFXs.emplace(id, ResourceManager::GetInstance().LoadSoundFX(path));
		}

		void PlaySoundFX(int id)
		{
			assert((m_tailIdx + 1) % m_maxPending != m_headIdx);

			for (const auto& message : m_pendingMessages)
				if (message.id == id) return;

			std::lock_guard lock(m_mutex);
			m_pendingMessages[m_tailIdx].id = id;
			m_tailIdx = (m_tailIdx + 1) % m_maxPending;

			m_processMessages = true;
			m_signalPromise.set_value();
		}

		void UpdateSoundFX()
		{
			while (!m_shouldQuit)
			{
				m_signalEnd.wait();

				while (m_processMessages)
				{
					if (m_headIdx == m_tailIdx) continue;

					if (Mix_PlayChannel(static_cast<int>(ChannelUse::SoundFX), m_soundFXs[m_pendingMessages[m_headIdx].id]->GetSoundFX(), 0) == -1)
					{
						Mix_FreeChunk(m_soundFXs[m_pendingMessages[m_headIdx].id]->GetSoundFX());
						Mix_Quit();
						throw std::runtime_error(std::string("Failed to play soundFX: ") + Mix_GetError());
					}

					std::lock_guard lock(m_mutex);

					m_pendingMessages[m_headIdx].id = static_cast<int>(sound::SoundId::Empty);
					m_headIdx = (m_headIdx + 1) % m_maxPending;

					if (m_pendingMessages[m_headIdx].id == static_cast<int>(sound::SoundId::Empty)) m_processMessages = false;
				}

				m_signalPromise = std::promise<void>{};
				m_signalEnd = m_signalPromise.get_future();
			}
		}

		void SignalEnd()
		{
			m_shouldQuit = true;
			m_signalPromise.set_value();
		}

	private:
		std::unordered_map<int, std::unique_ptr<SoundFX>> m_soundFXs{};

		static const int m_maxPending{ 16 };
		PlayMessage m_pendingMessages[m_maxPending]{};
		int m_headIdx{};
		int m_tailIdx{};

		bool m_processMessages{ false };
		bool m_shouldQuit{ false };

		std::promise<void> m_signalPromise;
		std::future<void> m_signalEnd;

		std::mutex m_mutex{};
	};

	SoundSystem::SoundSystem()
		: m_pImpl{ std::make_unique<SoundImpl>() }
	{

	}

	SoundSystem::~SoundSystem() = default;

	void SoundSystem::LoadSoundFX(int id, const std::string& path)
	{
		m_pImpl->LoadSoundFX(id, path);
	}

	void SoundSystem::PlaySoundFX(int id)
	{
		m_pImpl->PlaySoundFX(id);
	}

	void SoundSystem::UpdateSoundFX()
	{
		m_pImpl->UpdateSoundFX();
	}

	void SoundSystem::SignalEnd()
	{
		m_pImpl->SignalEnd();
	}
}