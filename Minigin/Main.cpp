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

	auto go = std::make_shared<dae::GameObject>();

	//background
	go.get()->AddComponent(std::make_shared<dae::RenderComponent>());
	auto bitmapComponent = go.get()->GetComponent<dae::RenderComponent>();
	if (bitmapComponent) bitmapComponent.get()->SetTexture("background.tga");

	//logo
	go.get()->AddComponent(std::make_shared<dae::RenderComponent>());
	auto logoComponent = go.get()->GetComponent<dae::RenderComponent>(2);
	if (logoComponent)
	{
		logoComponent.get()->SetTexture("logo.tga");
		logoComponent.get()->SetPosition(216, 180);
	}

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//text
	go.get()->AddComponent(std::make_shared<dae::TextComponent>());
	auto textComponent = go.get()->GetComponent<dae::TextComponent>();
	if (textComponent)
	{
		textComponent.get()->Initialize("Programming 4 Assignment", font);
		textComponent.get()->SetPosition(80, 20);
	}

	//fps
	go.get()->AddComponent(std::make_shared<dae::FPSComponent>());
	auto fpsComponent = go.get()->GetComponent<dae::FPSComponent>();
	if (fpsComponent)
	{
		fpsComponent.get()->Initialize(" ", font);
		fpsComponent.get()->SetPosition(10, 440);
	}

	scene.AddGameObject(go);
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