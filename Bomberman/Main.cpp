#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SDL_egl.h"

#include "ServiceLocator.h"
#include "Sound/LoggingSoundSystem.h"

#include "Bomberman.h"

namespace fs = std::filesystem;

void load()
{
#if _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::make_unique<dae::SoundSystem>()));
#else
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SoundSystem>());
#endif

	dae::Bomberman::GetInstance().LoadMainScene();
}

int main(int, char*[])
{
	dae::Minigin engine("../Data/", constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, constants::WINDOW_SCALE);
	engine.Run(load);
    return 0;
}