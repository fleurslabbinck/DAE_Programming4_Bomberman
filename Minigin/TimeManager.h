#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>
#include "Singleton.h"

namespace dae
{
	using namespace std::chrono;

	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		void SetCurrTime() { m_currentTime = high_resolution_clock::now(); }
		void SetLastTime() { m_lastTime = m_currentTime; }
		void CalculateDeltaTime() { m_deltaTime = duration<float>(m_currentTime - m_lastTime).count(); }
		void SetLag(float value) { m_lag += value; }

		float GetDeltaTime() const { return m_deltaTime; }
		float GetLag() const { return m_lag; }
		float GetFixedTimeStep() const { return m_fixedTimeStep; }
		nanoseconds GetSleepTime() const { return m_currentTime + m_msPerFrame - high_resolution_clock::now(); }

		void SetTargetFPS(int fps)
		{
			m_targetFPS = fps;
			m_msPerFrame = (milliseconds)static_cast<long long>(1000.f / m_targetFPS);
		}

	private:
		high_resolution_clock::time_point m_lastTime{};
		high_resolution_clock::time_point m_currentTime{};
		float m_deltaTime{};
		const float m_fixedTimeStep{ 0.3f };
		float m_lag{};

		int m_targetFPS{};
		milliseconds m_msPerFrame{};
	};
}
#endif