#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>

#include "Minigin.h"
#include "SDL_egl.h"

#include "BombermanUtil.h"
#include "BombermanManager.h"
#include "States/MainMenuState.h"

namespace fs = std::filesystem;

void load()
{
	dae::BombermanManager::GetInstance().SetState(new dae::MainMenuState());
	dae::BombermanManager::GetInstance().LoadScene(static_cast<int>(scenes::Scenes::Menu));
}

int main(int, char*[])
{
	std::cout << "INFO:\tPRESS I\n\n";

	fs::path data_location = "./Data/";
	if (!fs::exists(data_location)) data_location = "../Data/";

	dae::Minigin engine(data_location, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, constants::WINDOW_SCALE);
	engine.Run(load);
    return 0;
}