#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameObject.h"
#include "Scene.h"
#include "SDL_egl.h"

namespace fs = std::filesystem;

std::unique_ptr<dae::GameObject> GetFPSComponent(const std::string& font, const glm::vec2& pos);

void LoadTest(const std::string& font);

void LoadBombermanScene(const std::string& font);

void load()
{
	const std::string font{ "Lingua.otf" };

	//LoadTest(font);
	LoadBombermanScene(font);
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

std::unique_ptr<dae::GameObject> GetFPSComponent(const std::string& font, const glm::vec2& pos)
{
	constexpr int fontSize{ 36 };
	auto fps = std::make_unique<dae::GameObject>(pos.x, pos.y);
	fps->AddComponent(std::make_unique<dae::FPSComponent>(fps.get()));
	auto fpsComponent = fps->GetComponent<dae::FPSComponent>();
	if (fpsComponent) fpsComponent->Initialize(font, fontSize);

	return fps;
}

void LoadTest(const std::string& font)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	constexpr int fontSize{ 36 };

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

	//title
	auto title = std::make_unique<dae::GameObject>(80.f, 20.f);
	title->AddComponent(std::make_unique<dae::TextComponent>(title.get()));
	auto titleComponent = title->GetComponent<dae::TextComponent>();
	if (titleComponent) titleComponent->Initialize(font, fontSize, "Programming 4 Assignment");

	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(std::move(background));
	gameObjects.push_back(std::move(logo));
	gameObjects.push_back(std::move(title));
	gameObjects.push_back(std::move(GetFPSComponent(font, {})));

	for (auto& gameObj : gameObjects) scene.AddGameObject(std::move(gameObj));
}

void LoadBombermanScene(const std::string& font)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Bomberman");
	auto& input = dae::InputManager::GetInstance();

	constexpr int fontSize{ 20 };

	//background
	auto background = std::make_unique<dae::GameObject>();
	background->AddComponent(std::make_unique<dae::RenderComponent>(background.get()));
	auto bitmapComponent = background->GetComponent<dae::RenderComponent>();
	if (bitmapComponent)
	{
		bitmapComponent->SetTexture("Playfield.png");
		int textureWidth, textureHeight;
		SDL_QueryTexture(bitmapComponent->GetTexture()->GetSDLTexture(), NULL, NULL, &textureWidth, &textureHeight);
		background->SetPosition(0, WINDOW_HEIGHT * GRAPHICS_SCALE - static_cast<float>(textureHeight));
	}

	//instructions
	auto dpadDescr = std::make_unique<dae::GameObject>(10.f, 10.f);
	dpadDescr->AddComponent(std::make_unique<dae::TextComponent>(dpadDescr.get()));
	auto dpadDescrComp = dpadDescr->GetComponent<dae::TextComponent>();
	if (dpadDescrComp) dpadDescrComp->Initialize(font, fontSize, "Use the D-Pad to move normal Bomberman");

	auto wasdDescr = std::make_unique<dae::GameObject>(10.f, 35.f);
	wasdDescr->AddComponent(std::make_unique<dae::TextComponent>(wasdDescr.get()));
	auto wasdDescrComp = wasdDescr->GetComponent<dae::TextComponent>();
	if (wasdDescrComp) wasdDescrComp->Initialize(font, fontSize, "Use WASD to move Balloom");

	constexpr float speed{ 150.f };

	//gamepadcharacter
	auto bomberman = std::make_unique<dae::GameObject>(310.f, 300.f);
	bomberman->AddComponent(std::make_unique<dae::RenderComponent>(bomberman.get()));
	auto bombermanSprite = bomberman->GetComponent<dae::RenderComponent>();
	if (bombermanSprite)
	{
		bombermanSprite->SetTexture("Bomberman.png");
		int textureWidth, textureHeight;
		SDL_QueryTexture(bombermanSprite->GetTexture()->GetSDLTexture(), NULL, NULL, &textureWidth, &textureHeight);
		bomberman->SetPosition((WINDOW_WIDTH * GRAPHICS_SCALE - static_cast<float>(textureWidth)) / 3, (WINDOW_HEIGHT * GRAPHICS_SCALE - static_cast<float>(textureHeight)) / 2);
	}

	input.AddPlayerController(bomberman.get(), speed, dae::ControlMethod::Gamepad);

	//keyboardcharacter
	auto balloom = std::make_unique<dae::GameObject>(350.f, 300.f);
	balloom->AddComponent(std::make_unique<dae::RenderComponent>(balloom.get()));
	auto balloomSprite = balloom->GetComponent<dae::RenderComponent>();
	if (balloomSprite)
	{
		balloomSprite->SetTexture("Balloom.png");
		int textureWidth, textureHeight;
		SDL_QueryTexture(balloomSprite->GetTexture()->GetSDLTexture(), NULL, NULL, &textureWidth, &textureHeight);
		balloom->SetPosition(3 * (WINDOW_WIDTH * GRAPHICS_SCALE - static_cast<float>(textureWidth)) / 4, (WINDOW_HEIGHT * GRAPHICS_SCALE - static_cast<float>(textureHeight)) / 2);
	}

	input.AddPlayerController(balloom.get(), speed, dae::ControlMethod::Keyboard);

	/*auto balloom = std::make_unique<dae::GameObject>(10.f, 10.f);
	balloom->AddComponent(std::make_unique<dae::RenderComponent>(balloom.get()));
	balloom->AddComponent(std::make_unique<dae::RotatorComponent>(balloom.get(), 2.f));
	auto balloomSprite = balloom->GetComponent<dae::RenderComponent>();
	auto balloomRotator = balloom->GetComponent<dae::RotatorComponent>();
	if (balloomSprite && balloomRotator)
	{
		balloomSprite->SetTexture("Balloom.png");
	}

	balloom->SetParent(bomberman2.get());*/

	const glm::vec2 fpsPos{ 10.f, 70.f };

	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};
	gameObjects.push_back(std::move(background));
	gameObjects.push_back(std::move(dpadDescr));
	gameObjects.push_back(std::move(wasdDescr));
	gameObjects.push_back(std::move(bomberman));
	gameObjects.push_back(std::move(balloom));
	gameObjects.push_back(std::move(GetFPSComponent(font, fpsPos)));

	for (auto& gameObj : gameObjects) scene.AddGameObject(std::move(gameObj));
}