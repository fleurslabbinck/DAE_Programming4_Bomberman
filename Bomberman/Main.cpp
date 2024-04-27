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

#include "Bomberman.h"

namespace fs = std::filesystem;

void load()
{
	dae::ServiceLocator::RegisterSoundSystem();

	dae::Bomberman::GetInstance().LoadMainScene();
}

int main(int, char*[])
{
	dae::Minigin engine("../Data/", constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, constants::WINDOW_SCALE);
	engine.Run(load);
    return 0;
}