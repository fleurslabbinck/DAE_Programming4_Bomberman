#ifndef MINIGIN_H
#define MINIGIN_H

#include <string>
#include <functional>
#include <chrono>
#include <filesystem>

#include "Bomberman.h"
#include "Input/InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Render/Renderer.h"

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		Bomberman m_bomberman{};

		TimeManager& m_time = TimeManager::GetInstance();
		Renderer& m_renderer = Renderer::GetInstance();
		SceneManager& m_sceneManager = SceneManager::GetInstance();
		InputManager& m_input = InputManager::GetInstance();

		bool m_quit{ false };

		void RunOneFrame();
	};
}
#endif