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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();

	//background
	go.get()->AddComponent(std::make_shared<dae::BitmapComponent>());
	auto bitmapComponent = go.get()->GetComponent<dae::BitmapComponent>();
	if (bitmapComponent) bitmapComponent.get()->SetTexture("background.tga");

	//logo
	go.get()->AddComponent(std::make_shared<dae::BitmapComponent>());
	auto logoComponent = go.get()->GetComponent<dae::BitmapComponent>(2);
	if (logoComponent)
	{
		logoComponent.get()->SetTexture("logo.tga");
		logoComponent.get()->SetPosition(216, 180);
	}

	//text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go.get()->AddComponent(std::make_shared<dae::TextComponent>());
	auto textComponent = go.get()->GetComponent<dae::TextComponent>();
	if (textComponent)
	{
		textComponent.get()->Initialize("Programming 4 Assignment", font);
		textComponent.get()->SetPosition(80, 20);
	}

	scene.AddGameObject(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}