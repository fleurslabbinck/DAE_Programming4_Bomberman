#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"
#include <filesystem>
#include "SDL_egl.h"

namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto background = std::make_unique<dae::GameObject>();
	background->AddComponent(std::make_unique<dae::RenderComponent>(background.get()));
	auto bitmapComponent = background->GetComponent<dae::RenderComponent>();
	if (bitmapComponent) bitmapComponent->SetTexture("background.tga");

	//logo
	auto logo = std::make_unique<dae::GameObject>();
	logo->AddComponent(std::make_unique<dae::RenderComponent>(logo.get()));
	auto logoComponent = logo->GetComponent<dae::RenderComponent>();
	if (logoComponent)
	{
		logo->SetPosition(216, 180);
		logoComponent->SetTexture("logo.tga");
	}

	const std::string font{ "Lingua.otf" };
	const int fontSize{ 36 };

	//title
	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent(std::make_unique<dae::TextComponent>(title.get()));
	auto titleComponent = title->GetComponent<dae::TextComponent>();
	if (titleComponent)
	{
		title->SetPosition(80, 20);
		titleComponent->Initialize(font, fontSize, "Programming 4 Assignment");
	}

	//fps
	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent(std::make_unique<dae::FPSComponent>(fps.get()));
	auto fpsComponent = fps->GetComponent<dae::FPSComponent>();
	if (fpsComponent)
	{
		fps->SetPosition(10, 440);
		fpsComponent->Initialize(font, fontSize);
	}

	auto pacman = std::make_unique<dae::GameObject>();
	pacman->AddComponent(std::make_unique<dae::RenderComponent>(pacman.get()));
	auto pacmanSprite = pacman->GetComponent<dae::RenderComponent>();
	if (pacman)
	{
		pacman->SetPosition(310, 300);
		pacmanSprite->SetTexture("pacman.png");
	}
	
	auto pinkGhost = std::make_unique<dae::GameObject>();
	pinkGhost->AddComponent(std::make_unique<dae::RenderComponent>(pinkGhost.get()));
	pinkGhost->AddComponent(std::make_unique<dae::RotatorComponent>(pinkGhost.get(), -3.f));
	auto pinkGhostSprite = pinkGhost->GetComponent<dae::RenderComponent>();
	auto pinkGhostRotator = pinkGhost->GetComponent<dae::RotatorComponent>();
	if (pinkGhostSprite && pinkGhostRotator)
	{
		pinkGhost->SetPosition(25, 25);
		pinkGhostSprite->SetTexture("pinkGhost.png");
	}
	
	auto blueGhost = std::make_unique<dae::GameObject>();
	blueGhost->AddComponent(std::make_unique<dae::RenderComponent>(blueGhost.get()));
	blueGhost->AddComponent(std::make_unique<dae::RotatorComponent>(blueGhost.get(), 2.f));
	auto blueGhostSprite = blueGhost->GetComponent<dae::RenderComponent>();
	auto blueGhostRotator = blueGhost->GetComponent<dae::RotatorComponent>();
	if (blueGhostSprite && blueGhostRotator)
	{
		blueGhost->SetPosition(25, 25);
		blueGhostSprite->SetTexture("blueGhost.png");
	}
	
	auto orangeGhost = std::make_unique<dae::GameObject>();
	orangeGhost->AddComponent(std::make_unique<dae::RenderComponent>(orangeGhost.get()));
	orangeGhost->AddComponent(std::make_unique<dae::RotatorComponent>(orangeGhost.get(), -3.f));
	auto orangeGhostSprite = orangeGhost->GetComponent<dae::RenderComponent>();
	auto orangeGhostRotator = orangeGhost->GetComponent<dae::RotatorComponent>();
	if (orangeGhostSprite && orangeGhostRotator)
	{
		orangeGhost->SetPosition(50, 50);
		orangeGhostSprite->SetTexture("orangeGhost.png");
	}
	
	auto redGhost = std::make_unique<dae::GameObject>();
	redGhost->AddComponent(std::make_unique<dae::RenderComponent>(redGhost.get()));
	redGhost->AddComponent(std::make_unique<dae::RotatorComponent>(redGhost.get(), 5.f));
	auto redGhostSprite = redGhost->GetComponent<dae::RenderComponent>();
	auto redGhostRotator = redGhost->GetComponent<dae::RotatorComponent>();
	if (redGhostSprite && redGhostRotator)
	{
		redGhost->SetPosition(20, 20);
		redGhostSprite->SetTexture("redGhost.png");
	}
	
	pinkGhost->SetParent(pacman.get());
	blueGhost->SetParent(pinkGhost.get());
	orangeGhost->SetParent(blueGhost.get());
	redGhost->SetParent(orangeGhost.get());
	
	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(std::move(background));
	gameObjects.push_back(std::move(logo));
	gameObjects.push_back(std::move(title));
	gameObjects.push_back(std::move(fps));
	gameObjects.push_back(std::move(pacman));
	gameObjects.push_back(std::move(pinkGhost));
	gameObjects.push_back(std::move(blueGhost));
	gameObjects.push_back(std::move(orangeGhost));
	gameObjects.push_back(std::move(redGhost));

	for (auto& gameObj : gameObjects) scene.AddGameObject(std::move(gameObj));
}

int main(int, char*[]) {
#if __EMSCRITPEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif

#ifdef WIN32
	if (AllocConsole()) {
		FILE* pEmpty;
		freopen_s(&pEmpty, "CONOUT$", "w", stdout);
		freopen_s(&pEmpty, "CONOUT$", "w", stderr);
	}
#endif

	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}