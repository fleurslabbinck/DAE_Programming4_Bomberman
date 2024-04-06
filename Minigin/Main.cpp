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
#include "Components/CollisionComponent.h"
#include "Components/HealthComponent.h"
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
	fps->AddComponent<dae::FPSComponent>(fps.get(), font, fontSize);

	return fps;
}

void LoadBombermanScene(const std::string& font)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Bomberman");

	constexpr int fontSize{ 20 };

	std::vector<std::unique_ptr<dae::GameObject>> gameObjects{};

	//grid
	auto grid = std::make_unique<dae::GameObject>(0.f, static_cast<float>(WINDOW_HEIGHT - GRIDCELL * GRID_ROWS));
	grid->AddComponent<dae::GridComponent>(grid.get(), GRID_COLS, GRID_ROWS);
	dae::GameObject* parent{ grid.get() };
	gameObjects.push_back(std::move(grid));

	//playfield
	AssemblePlayField(gameObjects, parent);

	//instructions
	auto dpadDescr = std::make_unique<dae::GameObject>(10.f, 10.f);
	dpadDescr->AddComponent<dae::TextComponent>(dpadDescr.get(), font, fontSize, "USE D-PAD TO MOVE BOMBERMAN");
	gameObjects.push_back(std::move(dpadDescr));

	auto wasdDescr = std::make_unique<dae::GameObject>(10.f, 35.f);
	wasdDescr->AddComponent<dae::TextComponent>(wasdDescr.get(), font, fontSize, "USE WASD TO MOVE BALLOOM");
	gameObjects.push_back(std::move(wasdDescr));

	constexpr float speed{ 40 * WINDOW_SCALE };

	//gamepadcharacter
	auto bomberman = std::make_unique<dae::GameObject>(1.f * GRIDCELL, 1.f * GRIDCELL);
	const dae::RenderComponent* bombermanSprite{ bomberman->AddComponent<dae::RenderComponent>(bomberman.get(), "Bomberman.png") };
	dae::CollisionComponent* collisionComponent1{ bomberman->AddComponent<dae::CollisionComponent>(bomberman.get(), dae::CollisionComponent::EntityType::Player, bombermanSprite->GetDimensions()) };
	dae::HealthComponent* healthComponent{ bomberman->AddComponent<dae::HealthComponent>(bomberman.get(), 3) };
	collisionComponent1->AddObserver(healthComponent);

	AssignCommands(bomberman, dae::PlayerController::ControlMethod::Gamepad, speed);
	bomberman->SetParent(parent);
	gameObjects.push_back(std::move(bomberman));
	
	//keyboardcharacter
	auto balloom = std::make_unique<dae::GameObject>(9.f * GRIDCELL, 5.f * GRIDCELL);
	const dae::RenderComponent* balloomSprite{ balloom->AddComponent<dae::RenderComponent>(balloom.get(), "Balloom.png") };
	dae::CollisionComponent* collisionComponent2{ balloom->AddComponent<dae::CollisionComponent>(balloom.get(), dae::CollisionComponent::EntityType::Enemy, balloomSprite->GetDimensions()) };
	collisionComponent2->AddObserver(healthComponent);
	
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

	glm::vec2 startPos{};

	for (int row{}; row < GRID_ROWS; ++row)
		for (int col{}; col < GRID_COLS; ++col)
		{
			const int idx{ row * GRID_COLS + col };

			if (playfieldArr[idx] != '#') continue;

			startPos = parent->GetComponent<dae::GridComponent>()->GetCelPosAtIdx(idx);

			auto block = std::make_unique<dae::GameObject>(startPos.x, startPos.y);
			const dae::RenderComponent* blockSprite{ block->AddComponent<dae::RenderComponent>(block.get(), "Block.png") };
			block->AddComponent<dae::CollisionComponent>(block.get(), dae::CollisionComponent::EntityType::Block, blockSprite->GetDimensions());
			
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