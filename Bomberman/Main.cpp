#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SDL_egl.h"

#include "Bomberman.h"

namespace fs = std::filesystem;

void load()
{
	dae::Bomberman::GetInstance().LoadMainScene();
}

int main(int, char*[])
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location)) data_location = "../Data/";

	dae::Minigin engine(data_location, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, constants::WINDOW_SCALE);
	engine.Run(load);
    return 0;
}