#pragma once
#include <string>
#include <functional>
#include <chrono>
#include <filesystem>

#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		void SetTargetFPS(float fps);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		dae::Renderer& m_renderer = Renderer::GetInstance();
		dae::SceneManager& m_sceneManager = SceneManager::GetInstance();
		dae::InputManager& m_input = InputManager::GetInstance();

		float m_targetFPS{ 165.f };
		std::chrono::milliseconds m_msPerFrame{};
		const float m_fixedTimeStep{ 0.3f };

		std::chrono::high_resolution_clock::time_point m_lastTime{};
		float m_lag{};

		bool m_quit{ false };

		void RunOneFrame();
	};
}