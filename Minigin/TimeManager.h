#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae
{
	using namespace std::chrono;

	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		void UpdateCurrTime() { m_currentTime = high_resolution_clock::now(); }
		void SetLastTime() { m_lastTime = m_currentTime; }
		void CalculateDeltaTime() { m_deltaTime = duration<float>(m_currentTime - m_lastTime).count(); }

		float GetDeltaTime() const { return m_deltaTime; }
		high_resolution_clock::time_point GetCurrTime() const { return m_currentTime; }
		nanoseconds GetSleepTime() const { return m_currentTime + m_msPerFrame - high_resolution_clock::now(); }

		void SetTargetFPS(int fps)
		{
			m_targetFPS = fps;
			m_msPerFrame = (milliseconds)static_cast<long long>(1000.f / m_targetFPS);
		}

		milliseconds GetMsPerFrame() const { return m_msPerFrame; }

	private:
		high_resolution_clock::time_point m_lastTime{};
		high_resolution_clock::time_point m_currentTime{};
		float m_deltaTime{};

		int m_targetFPS{};
		milliseconds m_msPerFrame{};
	};
}