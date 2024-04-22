#include <stdexcept>
#include <sstream>
#include <iostream>
#include <thread>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "Render/Resources/ResourceManager.h"

#include "Bomberman.h"


SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
	std::stringstream ss;
	ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	LogSDLVersion("We compiled against SDL version ", version);

	SDL_GetVersion(&version);
	LogSDLVersion("We linked against SDL version ", version);

	SDL_IMAGE_VERSION(&version);
	LogSDLVersion("We compiled against SDL_image version ", version);

	version = *IMG_Linked_Version();
	LogSDLVersion("We linked against SDL_image version ", version);

	SDL_TTF_VERSION(&version)
	LogSDLVersion("We compiled against SDL_ttf version ", version);

	version = *TTF_Linked_Version();
	LogSDLVersion("We linked against SDL_ttf version ", version);
}

namespace dae
{
	Minigin::Minigin(const std::filesystem::path& dataPath, int windowWidth, int windowHeight)
	{
		PrintSDLVersion();

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}

		g_window = SDL_CreateWindow(
			"Bomberman - Fleur Slabbinck",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_OPENGL
		);
		if (g_window == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		m_renderer.Init(g_window);
		ResourceManager::GetInstance().Init(dataPath);
	}

	Minigin::~Minigin()
	{
		m_renderer.Destroy();
		SDL_DestroyWindow(g_window);
		g_window = nullptr;
		SDL_Quit();
	}

	void Minigin::Run(const std::function<void()>& load)
	{
		load();

		//dae::Bomberman::GetInstance().LoadMainScene();

		SDL_RenderSetVSync(m_renderer.GetSDLRenderer(), true);

		//constexpr int targetFPS{ 165 };
		//
		//m_time.SetTargetFPS(targetFPS);

		m_time.SetCurrTime();
		m_time.SetLastTime();

#ifndef __EMSCRIPTEN__
		while (!m_quit)
			RunOneFrame();
#else
		emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
	}

	void Minigin::RunOneFrame()
	{
		m_time.SetCurrTime();
		m_time.CalculateDeltaTime();
		m_time.SetLastTime();
		m_time.SetLag(m_time.GetDeltaTime());

		m_quit = m_input.ProcessInput();

		const float m_fixedTimeStep{ m_time.GetFixedTimeStep() };

		while (m_time.GetLag() >= m_fixedTimeStep)
		{
			m_sceneManager.FixedUpdate();
			m_time.SetLag(-m_fixedTimeStep);
		}

		m_sceneManager.Update();
		m_sceneManager.LateUpdate();
		m_renderer.Render();
	}
}