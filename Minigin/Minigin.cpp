#include "Minigin.h"

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

#include "Render/Resources/ResourceManager.h"
#include "ServiceLocator.h"
#include "Sound/LoggingSoundSystem.h"


SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n", version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n", version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n", version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n", version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n", version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n", version.major, version.minor, version.patch);
}

namespace dae
{
	Minigin::Minigin(const std::filesystem::path& dataPath, int windowWidth, int windowHeight, int windowScale)
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
			windowWidth * windowScale,
			windowHeight * windowScale,
			SDL_WINDOW_OPENGL
		);

		if (g_window == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		m_renderer.Init(g_window, windowScale);
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
#if _DEBUG
		ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::make_unique<dae::SoundSystem>()));
#else
		ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SoundSystem>());
#endif

		load();

		SDL_RenderSetVSync(m_renderer.GetSDLRenderer(), true);

		std::jthread soundThread(&BaseSoundSystem::UpdateSoundFX, &ServiceLocator::GetSoundSystem());

		m_time.SetCurrTime();
		m_time.SetLastTime();

		while (!m_quit)
			RunOneFrame();

		ServiceLocator::GetSoundSystem().SignalEnd();
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