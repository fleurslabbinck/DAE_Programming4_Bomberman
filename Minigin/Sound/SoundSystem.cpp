#include "SoundSystem.h"

#include <SDL_mixer.h>
#include <unordered_map>
#include <deque>
#include <iostream>
#include <future>
#include <mutex>

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
			: m_signalPromise{}
			, m_signalEnd{ m_signalPromise.get_future() }
		{
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) throw std::runtime_error(std::string("Failed to initialize SDL Mixer: ") + Mix_GetError());
		}

		~SoundImpl()
		{
			Mix_CloseAudio();
		}

		void LoadSoundFX(int id, const std::string& path)
		{
			m_soundFXs.emplace(id, ResourceManager::GetInstance().LoadSoundFX(path));
		}

		void PlaySoundFX(int id, int volume)
		{
			std::deque<PlayMessage> currentMessages = m_pendingMessages;

			for (const PlayMessage& message : m_pendingMessages)
			{
				if (id == message.id) return;
			}

			std::lock_guard lock(m_mutex);

			m_pendingMessages.push_back(PlayMessage{ id, volume });

			if (m_promiseSet) return;

			m_processMessages = true;
			m_signalPromise.set_value();
			m_promiseSet = true;
		}

		void UpdateSoundFX()
		{
			while (!m_shouldQuit)
			{
				// Wait for value to be set
				m_signalEnd.wait();

				// Lock access to pending messages
				m_mutex.lock();

				// Copy messages
				std::deque<PlayMessage> currentMessages = m_pendingMessages;

				// Empty messages that will be handled
				m_pendingMessages.clear();

				// Unlock to handle local messages
				m_mutex.unlock();

				while (m_processMessages)
				{
					// Play sounds untill empty
					while (!currentMessages.empty())
					{
						Mix_Volume(static_cast<int>(ChannelUse::SoundFX), currentMessages.front().volume);
						if (Mix_PlayChannel(static_cast<int>(ChannelUse::SoundFX), m_soundFXs[currentMessages.front().id]->GetSoundFX(), 0) == -1)
						{
							Mix_FreeChunk(m_soundFXs[currentMessages.front().id]->GetSoundFX());
							Mix_Quit();
							throw std::runtime_error(std::string("Failed to play soundFX: ") + Mix_GetError());
						}

						currentMessages.pop_front();
					}

					// Get out of while loop when messages handled
					m_processMessages = false;
				}

				std::lock_guard<std::mutex> promiseLock(m_mutex);

				m_signalPromise = std::promise<void>{};
				m_signalEnd = m_signalPromise.get_future();
				m_promiseSet = false;
			}
		}

		void SignalEnd()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			
			m_shouldQuit = true;
			m_signalPromise.set_value();
		}

	private:
		std::unordered_map<int, std::unique_ptr<SoundFX>> m_soundFXs{};
		std::deque<PlayMessage> m_pendingMessages{};

		bool m_processMessages{ false };
		bool m_promiseSet{ false };
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

	void SoundSystem::PlaySoundFX(int id, int volume)
	{
		m_pImpl->PlaySoundFX(id, volume);
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