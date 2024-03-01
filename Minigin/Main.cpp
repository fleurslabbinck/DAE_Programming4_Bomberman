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

namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent(std::make_shared<dae::RenderComponent>(background.get()));
	auto bitmapComponent = background->GetComponent<dae::RenderComponent>();
	if (bitmapComponent) bitmapComponent->SetTexture("background.tga");

	//logo
	auto logo = std::make_shared<dae::GameObject>();
	logo->AddComponent(std::make_shared<dae::RenderComponent>(logo.get()));
	auto logoComponent = logo->GetComponent<dae::RenderComponent>();
	if (logoComponent)
	{
		logo->SetPosition(216, 180);
		logoComponent->SetTexture("logo.tga");
	}

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//title
	auto title = std::make_shared<dae::GameObject>();
	title->AddComponent(std::make_shared<dae::TextComponent>(title.get()));
	auto titleComponent = title->GetComponent<dae::TextComponent>();
	if (titleComponent)
	{
		title->SetPosition(80, 20);
		titleComponent->Initialize(font, "Programming 4 Assignment");
	}

	//fps
	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent(std::make_shared<dae::FPSComponent>(fps.get()));
	auto fpsComponent = fps->GetComponent<dae::FPSComponent>();
	if (fpsComponent)
	{
		fps->SetPosition(10, 440);
		fpsComponent->Initialize(font);
	}

	std::vector<std::shared_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(background);
	gameObjects.push_back(logo);
	gameObjects.push_back(title);
	gameObjects.push_back(fps);

	for (auto& gameObj : gameObjects) scene.AddGameObject(gameObj);
}

int main(int, char*[]) {
#if __EMSCRITPEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}