#ifndef MINIGIN_H
#define MINIGIN_H

#include <string>
#include <functional>
#include <filesystem>

#include "TimeManager.h"
#include "Input/InputManager.h"
#include "Scene/SceneManager.h"
#include "Render/Renderer.h"

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath, int windowWidth, int windowHeight, int windowScale);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		TimeManager& m_time = TimeManager::GetInstance();
		Renderer& m_renderer = Renderer::GetInstance();
		SceneManager& m_sceneManager = SceneManager::GetInstance();
		InputManager& m_input = InputManager::GetInstance();

		bool m_quit{ false };

		void RunOneFrame();
	};
}
#endif