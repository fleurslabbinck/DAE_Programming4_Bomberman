#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameObject.h"
#include "Components/RotatorComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/GridComponent.h"
#include "Scene.h"
#include "SDL_egl.h"

namespace fs = std::filesystem;

std::unique_ptr<dae::GameObject> GetFPSComponent(const std::string& font, const glm::vec2& pos);

void LoadBombermanScene(const std::string& font);

void AssemblePlayField(std::vector<std::unique_ptr<dae::GameObject>>& gameObjects, dae::GameObject* parent);

void AssignCommands(std::unique_ptr<dae::GameObject>& gameObject, dae::PlayerController::ControlMethod controlMethod, float speed);

void load()
{
	const std::string font{ "nintendo-nes-font.otf" };

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

void LoadBombermanScene(const std::string& font)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Bomberman");

	constexpr int fontSize{ 20 };

	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};

	//grid
	auto grid = std::make_unique<dae::GameObject>(0.f, static_cast<float>(WINDOW_HEIGHT - GRIDCELL * (GRID_ROWS + 1)));
	grid->AddComponent(std::make_unique<dae::GridComponent>(grid.get(), GRID_COLS, GRID_ROWS));
	dae::GameObject* parent{ grid.get() };
	gameObjects.push_back(std::move(grid));

	//playfield
	AssemblePlayField(gameObjects, parent);

	//instructions
	auto dpadDescr = std::make_unique<dae::GameObject>(10.f, 10.f);
	dpadDescr->AddComponent(std::make_unique<dae::TextComponent>(dpadDescr.get()));
	auto dpadDescrComp = dpadDescr->GetComponent<dae::TextComponent>();
	if (dpadDescrComp) dpadDescrComp->Initialize(font, fontSize, "USE D-PAD TO MOVE BOMBERMAN");
	gameObjects.push_back(std::move(dpadDescr));

	auto wasdDescr = std::make_unique<dae::GameObject>(10.f, 35.f);
	wasdDescr->AddComponent(std::make_unique<dae::TextComponent>(wasdDescr.get()));
	auto wasdDescrComp = wasdDescr->GetComponent<dae::TextComponent>();
	if (wasdDescrComp) wasdDescrComp->Initialize(font, fontSize, "USE WASD TO MOVE BALLOOM");
	gameObjects.push_back(std::move(wasdDescr));

	constexpr float speed{ 40 * WINDOW_SCALE };

	//gamepadcharacter
	auto bomberman = std::make_unique<dae::GameObject>(3.f * GRIDCELL, 5.f * GRIDCELL);
	bomberman->AddComponent(std::make_unique<dae::RenderComponent>(bomberman.get()));
	auto bombermanSprite = bomberman->GetComponent<dae::RenderComponent>();
	if (bombermanSprite)
	{
		bombermanSprite->SetTexture("Bomberman.png");
	}
	AssignCommands(bomberman, dae::PlayerController::ControlMethod::Gamepad, speed);
	bomberman->SetParent(parent);
	gameObjects.push_back(std::move(bomberman));
	
	//keyboardcharacter
	auto balloom = std::make_unique<dae::GameObject>(10.f * GRIDCELL, 5.f * GRIDCELL);
	balloom->AddComponent(std::make_unique<dae::RenderComponent>(balloom.get()));
	auto balloomSprite = balloom->GetComponent<dae::RenderComponent>();
	if (balloomSprite)
	{
		balloomSprite->SetTexture("Balloom.png");
	}
	AssignCommands(balloom, dae::PlayerController::ControlMethod::Gamepad, speed);
	AssignCommands(balloom, dae::PlayerController::ControlMethod::Keyboard, speed);

	balloom->SetParent(parent);
	gameObjects.push_back(std::move(balloom));

	//fps component
	const glm::vec2 fpsPos{ 10.f, 70.f };

	gameObjects.push_back(std::move(GetFPSComponent(font, fpsPos)));

	//add gameobjects to scene
	for (auto& gameObj : gameObjects) scene.AddGameObject(std::move(gameObj));
}

void AssemblePlayField(std::vector<std::unique_ptr<dae::GameObject>>& gameObjects, dae::GameObject* parent)
{
	const char playfieldArr[GRID_COLS * GRID_ROWS]{
		'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
		'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
		'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
	};

	const glm::vec2 parentPos{ parent->GetTransform()->GetLocalPosition() };

	glm::vec2 startPos{};

	for (int row{}; row < GRID_ROWS; ++row)
		for (int col{}; col < GRID_COLS; ++col)
		{
			const int idx{ row * GRID_COLS + col };

			if (playfieldArr[idx] != '#') continue;

			startPos.x = parentPos.x + static_cast<float>(GRIDCELL * col);
			startPos.y = parentPos.y + static_cast<float>(GRIDCELL * row);

			auto block = std::make_unique<dae::GameObject>(startPos.x, startPos.y);
			block->AddComponent(std::make_unique<dae::RenderComponent>(block.get()));
			auto blockSprite = block->GetComponent<dae::RenderComponent>();
			if (blockSprite) blockSprite->SetTexture("Block.png");

			block->SetParent(parent);
			gameObjects.push_back(std::move(block));
		}
}

void AssignCommands(std::unique_ptr<dae::GameObject>& gameObject, dae::PlayerController::ControlMethod controlMethod, float speed)
{
	switch (controlMethod)
	{
	case dae::PlayerController::ControlMethod::Gamepad:
	{
		dae::PlayerController* player{ dae::InputManager::GetInstance().AddPlayerController(dae::PlayerController::ControlMethod::Gamepad) };
		player->BindCommand(static_cast<int>(dae::GamepadButton::DPadLeft), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ -1, 0 }));
		player->BindCommand(static_cast<int>(dae::GamepadButton::DPadRight), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 1, 0 }));
		player->BindCommand(static_cast<int>(dae::GamepadButton::DPadDown), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 0, 1 }));
		player->BindCommand(static_cast<int>(dae::GamepadButton::DPadUp), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 0, -1 }));
		break;
	}
	case dae::PlayerController::ControlMethod::Keyboard:
	{
		dae::PlayerController* player{ dae::InputManager::GetInstance().AddPlayerController(dae::PlayerController::ControlMethod::Keyboard) };
		player->BindCommand(static_cast<int>(SDL_SCANCODE_A), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ -1, 0 }));
		player->BindCommand(static_cast<int>(SDL_SCANCODE_D), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 1, 0 }));
		player->BindCommand(static_cast<int>(SDL_SCANCODE_S), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 0, 1 }));
		player->BindCommand(static_cast<int>(SDL_SCANCODE_W), std::make_unique<dae::MoveCommand>(gameObject.get(), speed, glm::vec2{ 0, -1 }));
		break;
	}
	}
}