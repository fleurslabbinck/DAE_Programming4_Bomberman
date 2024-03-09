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

	auto bomberman = std::make_unique<dae::GameObject>(310.f, 300.f);
	bomberman->AddComponent(std::make_unique<dae::RenderComponent>(bomberman.get()));
	auto bombermanSprite = bomberman->GetComponent<dae::RenderComponent>();
	if (bombermanSprite) bombermanSprite->SetTexture("Bomberman.png");
	
	auto balloom = std::make_unique<dae::GameObject>(25.f, 25.f);
	balloom->AddComponent(std::make_unique<dae::RenderComponent>(balloom.get()));
	balloom->AddComponent(std::make_unique<dae::RotatorComponent>(balloom.get(), -3.f));
	auto balloomSprite = balloom->GetComponent<dae::RenderComponent>();
	if (balloomSprite) balloomSprite->SetTexture("Balloom.png");
	
	auto oneal = std::make_unique<dae::GameObject>(25.f, 25.f);
	oneal->AddComponent(std::make_unique<dae::RenderComponent>(oneal.get()));
	oneal->AddComponent(std::make_unique<dae::RotatorComponent>(oneal.get(), 2.f));
	auto onealSprite = oneal->GetComponent<dae::RenderComponent>();
	if (onealSprite) onealSprite->SetTexture("Oneal.png");
	
	auto doll = std::make_unique<dae::GameObject>(50.f, 50.f);
	doll->AddComponent(std::make_unique<dae::RenderComponent>(doll.get()));
	doll->AddComponent(std::make_unique<dae::RotatorComponent>(doll.get(), -3.f));
	auto dollSprite = doll->GetComponent<dae::RenderComponent>();
	if (dollSprite) dollSprite->SetTexture("Doll.png");
	
	auto minvo = std::make_unique<dae::GameObject>(20.f, 20.f);
	minvo->AddComponent(std::make_unique<dae::RenderComponent>(minvo.get()));
	minvo->AddComponent(std::make_unique<dae::RotatorComponent>(minvo.get(), 5.f));
	auto minvoSprite = minvo->GetComponent<dae::RenderComponent>();
	if (minvoSprite) minvoSprite->SetTexture("Minvo.png");
	
	balloom->SetParent(bomberman.get());
	oneal->SetParent(balloom.get());
	doll->SetParent(oneal.get());
	minvo->SetParent(doll.get());
	
	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(std::move(background));
	gameObjects.push_back(std::move(logo));
	gameObjects.push_back(std::move(title));
	gameObjects.push_back(std::move(fps));
	gameObjects.push_back(std::move(bomberman));
	gameObjects.push_back(std::move(balloom));
	gameObjects.push_back(std::move(oneal));
	gameObjects.push_back(std::move(doll));
	gameObjects.push_back(std::move(minvo));

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