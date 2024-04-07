#include "Bomberman.h"

#include <SDL.h>
#include "SceneManager.h"
#include "GameObject.h"
#include "Components/RotatorComponent.h"
#include "Components/RenderComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/GridComponent.h"
#include "Components/HealthComponent.h"

namespace dae
{
	void Bomberman::LoadMainScene()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");

		const std::string font{ "nintendo-nes-font.otf" };
		constexpr int fontSize{ 2 * WINDOW_SCALE };

		// Instructions
		GameObject* dpadDescr{ scene.AddGameObject(std::make_unique<dae::GameObject>(10.f, 10.f)) };
		dpadDescr->AddComponent<dae::TextComponent>(font, fontSize, "USE D-PAD TO MOVE BOMBERMAN");

		GameObject* wasdDescr{ scene.AddGameObject(std::make_unique<dae::GameObject>(10.f, 12.f * WINDOW_SCALE)) };
		wasdDescr->AddComponent<dae::TextComponent>(font, fontSize, "USE WASD TO MOVE BALLOOM");

		// Playfield
		GameObject* playfield{ scene.AddGameObject(std::make_unique<GameObject>(0.f, static_cast<float>(WINDOW_HEIGHT - GRIDCELL * GRID_ROWS))) };
		playfield->AddComponent<GridComponent>(GRID_COLS, GRID_ROWS);
		InitializePlayfield(playfield, scene, GRID_COLS, GRID_ROWS);

		// Player1
		const glm::vec2 playerStartPos{ 1.f * GRIDCELL, 1.f * GRIDCELL };
		const glm::vec2 playerCollisionBox{ 10 * WINDOW_SCALE, 1.f * GRIDCELL };

		GameObject* bomberman{ InitializePlayer(playfield, scene, playerStartPos, CollisionComponent::EntityType::Player, playerCollisionBox, "Sprites/Bomberman.png") };
		HealthComponent* bombermanHealthComp{ bomberman->GetComponent<HealthComponent>() };
		
		// Enemy1
		const glm::vec2 enemyStartPos{ 10.f * GRIDCELL, 1.f * GRIDCELL };
		const glm::vec2 enemyCollisionBox{ 1.f * GRIDCELL, 1.f * GRIDCELL };

		GameObject* balloom{ InitializePlayer(playfield, scene, enemyStartPos, CollisionComponent::EntityType::Enemy, enemyCollisionBox, "Sprites/Balloom.png") };
		CollisionComponent* balloomCollisionComp{ balloom->GetComponent<CollisionComponent>() };

		balloomCollisionComp->AddObserver(bombermanHealthComp);

		// Controls
		AddControls(bomberman, PlayerController::ControlMethod::Gamepad, m_speed);
		AddControls(balloom, PlayerController::ControlMethod::Keyboard, m_speed);
		AddControls(balloom, PlayerController::ControlMethod::Gamepad, m_speed);

		// FPS component
		scene.AddGameObject(GetFPSComponent(font, fontSize, m_fpsComponentPos));
	}

	void Bomberman::InitializePlayfield(GameObject* parent, Scene& scene, int totalCols, int totalRows) const
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

		const glm::vec2 blockCollisionBox{ static_cast<float>(GRIDCELL), static_cast<float>(GRIDCELL) };

		glm::vec2 startPos{};

		for (int row{}; row < totalRows; ++row)
			for (int col{}; col < totalCols; ++col)
			{
				//if (col % 2 == 1 || row % 2 == 1) continue;

				const int idx{ row * GRID_COLS + col };

				if (playfieldArr[idx] != '#') continue;

				startPos = parent->GetComponent<dae::GridComponent>()->GetCelPosAtIdx(idx);
				
				GameObject* block{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
				block->AddComponent<RenderComponent>("Obstacles/Block.png");
				block->AddComponent<CollisionComponent>(CollisionComponent::EntityType::Block, 0.f, blockCollisionBox);
				block->SetParent(parent);
			}
	}

	GameObject* Bomberman::InitializePlayer(GameObject* parent, Scene& scene, const glm::vec2& startPos, CollisionComponent::EntityType type, const glm::vec2& collisionBox, const std::string& filename) const
	{
		const float offset{ (GRIDCELL - collisionBox.x) / 2 };

		GameObject* player{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
		CollisionComponent* collisionComp{ player->AddComponent<CollisionComponent>(type, offset, collisionBox) };
		HealthComponent* healthComp{};

		if (type == CollisionComponent::EntityType::Player) 
		{
			player->AddComponent<SpriteComponent>(filename, 7, 3, 3, 7);
			healthComp = player->AddComponent<HealthComponent>(4);
		}
		else
		{
			player->AddComponent<SpriteComponent>(filename, 6, 2, 3, 6);
			healthComp = player->AddComponent<HealthComponent>(1);
		}

		collisionComp->AddObserver(healthComp);

		player->SetParent(parent);

		return player;
	}

	void Bomberman::AddControls(GameObject* gameObject, dae::PlayerController::ControlMethod controlMethod, float speed) const
	{
		switch (controlMethod)
		{
		case dae::PlayerController::ControlMethod::Gamepad:
		{
			dae::PlayerController* player{ dae::InputManager::GetInstance().AddPlayerController(dae::PlayerController::ControlMethod::Gamepad) };
			player->BindCommand(static_cast<int>(dae::GamepadButton::DPadLeft), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player->BindCommand(static_cast<int>(dae::GamepadButton::DPadRight), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player->BindCommand(static_cast<int>(dae::GamepadButton::DPadDown), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player->BindCommand(static_cast<int>(dae::GamepadButton::DPadUp), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			break;
		}
		case dae::PlayerController::ControlMethod::Keyboard:
		{
			dae::PlayerController* player{ dae::InputManager::GetInstance().AddPlayerController(dae::PlayerController::ControlMethod::Keyboard) };
			player->BindCommand(static_cast<int>(SDL_SCANCODE_A), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_D), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_S), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_W), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			break;
		}
		}
	}

	std::unique_ptr<GameObject> Bomberman::GetFPSComponent(const std::string& font, int fontSize, const glm::vec2& pos) const
	{
		auto fps = std::make_unique<dae::GameObject>(pos.x, pos.y);
		fps->AddComponent<dae::FPSComponent>(font, fontSize);
		return fps;
	}
}