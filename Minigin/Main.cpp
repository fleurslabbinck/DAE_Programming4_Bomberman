#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "InputManager.h"
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
	constexpr int nrControllers{ 2 };
	constexpr int nrGamepads{ 1 };

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();
	input.Init(nrControllers, nrGamepads);

	//background
	auto background = std::make_unique<dae::GameObject>();
	background->AddComponent(std::make_unique<dae::RenderComponent>(background.get()));
	auto bitmapComponent = background->GetComponent<dae::RenderComponent>();
	if (bitmapComponent) bitmapComponent->SetTexture("background.tga");

	//logo
	auto logo = std::make_unique<dae::GameObject>(216.f, 180.f);
	logo->AddComponent(std::make_unique<dae::RenderComponent>(logo.get()));
	auto logoComponent = logo->GetComponent<dae::RenderComponent>();
	if (logoComponent) logoComponent->SetTexture("logo.tga");

	const std::string font{ "Lingua.otf" };
	const int fontSize{ 36 };

	//title
	auto title = std::make_unique<dae::GameObject>(80.f, 20.f);
	title->AddComponent(std::make_unique<dae::TextComponent>(title.get()));
	auto titleComponent = title->GetComponent<dae::TextComponent>();
	if (titleComponent) titleComponent->Initialize(font, fontSize, "Programming 4 Assignment");

	//fps
	auto fps = std::make_unique<dae::GameObject>(10.f, 440.f);
	fps->AddComponent(std::make_unique<dae::FPSComponent>(fps.get()));
	auto fpsComponent = fps->GetComponent<dae::FPSComponent>();
	if (fpsComponent) fpsComponent->Initialize(font, fontSize);

	const int fontSize2{ 20 };
	
	//instructions
	auto dpadDescr = std::make_unique<dae::GameObject>(10.f, 100.f);
	dpadDescr->AddComponent(std::make_unique<dae::TextComponent>(dpadDescr.get()));
	auto dpadDescrComp = dpadDescr->GetComponent<dae::TextComponent>();
	if (dpadDescrComp) dpadDescrComp->Initialize(font, fontSize2, "Use the D-Pad to move normal Bomberman");

	auto wasdDescr = std::make_unique<dae::GameObject>(10.f, 125.f);
	wasdDescr->AddComponent(std::make_unique<dae::TextComponent>(wasdDescr.get()));
	auto wasdDescrComp = wasdDescr->GetComponent<dae::TextComponent>();
	if (wasdDescrComp) wasdDescrComp->Initialize(font, fontSize2, "Use WASD to move Ms green Bomberman");

	//gamepadcharacter
	auto bomberman = std::make_unique<dae::GameObject>(310.f, 300.f);
	bomberman->AddComponent(std::make_unique<dae::RenderComponent>(bomberman.get()));
	auto bombermanSprite = bomberman->GetComponent<dae::RenderComponent>();
	if (bombermanSprite) bombermanSprite->SetTexture("Bomberman.png");

	bomberman->SetMovementSpeed(100);
	input.BindCommand(bomberman.get());

	//keyboardcharacter
	auto bomberman2 = std::make_unique<dae::GameObject>(350.f, 300.f);
	bomberman2->AddComponent(std::make_unique<dae::RenderComponent>(bomberman2.get()));
	auto bombermanSprite2 = bomberman2->GetComponent<dae::RenderComponent>();
	if (bombermanSprite2) bombermanSprite2->SetTexture("Bomberman2.png");

	bomberman2->SetMovementSpeed(100);
	input.BindCommand(bomberman2.get());
	
	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(std::move(background));
	gameObjects.push_back(std::move(logo));
	gameObjects.push_back(std::move(title));
	gameObjects.push_back(std::move(fps));
	gameObjects.push_back(std::move(dpadDescr));
	gameObjects.push_back(std::move(wasdDescr));
	gameObjects.push_back(std::move(bomberman));
	gameObjects.push_back(std::move(bomberman2));

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