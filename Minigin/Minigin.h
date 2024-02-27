#pragma once
#include <string>
#include <functional>
#include <chrono>
#include <filesystem>

#include "TimeManager.h"
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

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		dae::TimeManager& m_time = TimeManager::GetInstance();
		dae::Renderer& m_renderer = Renderer::GetInstance();
		dae::SceneManager& m_sceneManager = SceneManager::GetInstance();
		dae::InputManager& m_input = InputManager::GetInstance();

		bool m_quit{ false };

		void RunOneFrame();
	};
}