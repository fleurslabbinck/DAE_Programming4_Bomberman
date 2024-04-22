#include "Bomberman.h"

#include "../Minigin/Scene/SceneManager.h"
#include "../Minigin/Objects/GameObject.h"
#include "../Minigin/Objects/Components/RenderComponent.h"
#include "../Minigin/Objects/Components/TextComponent.h"
#include "../Minigin/Objects/Components/FPSComponent.h"
#include "../Bomberman/Components/GridComponent.h"
#include "../Bomberman/Components/CameraComponent.h"
#include "../Bomberman/Components/HUDComponent.h"
#include "../Bomberman/Components/BomberComponent.h"
#include "../Bomberman/Commands/MoveCommand.h"
#include "../Bomberman/Commands/BombCommand.h"

namespace dae
{
	void Bomberman::LoadMainScene()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");

		const std::string font{ "nintendo-nes-font.otf" };
		constexpr int fontSize{ 8 };

		// Playfield
		GameObject* playfield{ scene.AddGameObject(std::make_unique<GameObject>(0.f, static_cast<float>(constants::WINDOW_HEIGHT - constants::GRIDCELL * constants::GRID_ROWS))) };
		playfield->AddComponent<GridComponent>(constants::GRID_COLS, constants::GRID_ROWS, true, m_backgroundColor);
		InitializePlayfield(playfield, scene, constants::GRID_COLS, constants::GRID_ROWS);

		// Player
		const glm::vec2 playerStartPos{ 1.f * constants::GRIDCELL, 1.f * constants::GRIDCELL };
		const glm::vec2 playerCollisionBox{ 10.f, 1.f * constants::GRIDCELL };

		GameObject* bomberman{ InitializePlayer(scene, playfield, playerStartPos,  playerCollisionBox, "Sprites/Bomberman.png") };
		HealthComponent* bombermanHealthComp{ bomberman->GetComponent<HealthComponent>() };

		// PlayerHUD
		HUDComponent* hudComp{ bomberman->AddComponent<HUDComponent>(font, fontSize) };
		bombermanHealthComp->AddObserver(hudComp);
		
		// Enemy
		const glm::vec2 enemyStartPos{ 10.f * constants::GRIDCELL, 1.f * constants::GRIDCELL };
		const glm::vec2 enemyCollisionBox{ 15.f, 1.f * constants::GRIDCELL };

		GameObject* balloom{ InitializeEnemy(scene, playfield, enemyStartPos, enemyCollisionBox, "Sprites/Balloom.png", SpriteComponent::SpriteType::BALLOOM, bombermanHealthComp) };
		CollisionComponent* balloomCollisionComp{ balloom->GetComponent<CollisionComponent>() };

		balloomCollisionComp->AddObserver(bombermanHealthComp);

		// Controls
		AddControls(bomberman, PlayerController::ControlMethod::Gamepad, m_speed);
		AddControls(balloom, PlayerController::ControlMethod::Keyboard, m_speed);
		//AddControls(balloom, PlayerController::ControlMethod::Gamepad, m_speed);

		// FPS component
		scene.AddGameObject(GetFPSComponent(font, fontSize, m_fpsComponentPos));
	}

	void Bomberman::InitializePlayfield(GameObject* parent, Scene& scene, int totalCols, int totalRows) const
	{
		const char playfieldArr[constants::GRID_COLS * constants::GRID_ROWS]{
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

		const glm::vec2 blockCollisionBox{ static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL) };

		glm::vec2 startPos{};

		for (int row{}; row < totalRows; ++row)
			for (int col{}; col < totalCols; ++col)
			{
				//if (col % 2 == 1 || row % 2 == 1) continue;

				const int idx{ row * constants::GRID_COLS + col };

				if (playfieldArr[idx] != '#') continue;

				startPos = parent->GetComponent<dae::GridComponent>()->GetCelPosAtIdx(idx);
				
				GameObject* block{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
				block->AddComponent<RenderComponent>("Obstacles/Block.png");
				block->AddComponent<CollisionComponent>(CollisionComponent::EntityType::Block, 0.f, blockCollisionBox);
				block->SetParent(parent);
			}
	}

	GameObject* Bomberman::InitializePlayer(Scene& scene, GameObject* parent, const glm::vec2& startPos, const glm::vec2& collisionBox, const std::string& filename) const
	{
		const float offset{ (constants::GRIDCELL - collisionBox.x) / 2 };

		GameObject* player{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
		CollisionComponent* collisionComp{ player->AddComponent<CollisionComponent>(CollisionComponent::EntityType::Player, offset, collisionBox) };
		SpriteComponent* spriteComp{ player->AddComponent<SpriteComponent>(filename, SpriteComponent::SpriteType::BOMBERMAN) };
		HealthComponent* healthComp{ player->AddComponent<HealthComponent>(3) };
		player->AddComponent<BomberComponent>(scene);
		player->AddComponent<CameraComponent>(constants::GRID_COLS * constants::GRIDCELL, 0, constants::GRID_COLS * constants::GRIDCELL - constants::WINDOW_WIDTH);
		collisionComp->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(collisionComp);
		spriteComp->AddObserver(healthComp);

		player->SetParent(parent);

		return player;
	}

	GameObject* Bomberman::InitializeEnemy(Scene& scene, GameObject* parent, const glm::vec2& startPos, const glm::vec2& collisionBox, const std::string& filename, SpriteComponent::SpriteType type, HealthComponent* playerHealthComp) const
	{
		const float offset{ (constants::GRIDCELL - collisionBox.x) / 2 };

		GameObject* enemy{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
		CollisionComponent* collisionComp{ enemy->AddComponent<CollisionComponent>(CollisionComponent::EntityType::Enemy, offset, collisionBox) };
		SpriteComponent* spriteComp{};

		switch (type)
		{
		case dae::SpriteComponent::SpriteType::BALLOOM:
			spriteComp = enemy->AddComponent<SpriteComponent>(filename, type, 100);
			break;
		case dae::SpriteComponent::SpriteType::ONEAL:
			spriteComp = enemy->AddComponent<SpriteComponent>(filename, type, 200);
			break;
		case dae::SpriteComponent::SpriteType::DOLL:
			spriteComp = enemy->AddComponent<SpriteComponent>(filename, type, 400);
			break;
		case dae::SpriteComponent::SpriteType::MINVO:
			spriteComp = enemy->AddComponent<SpriteComponent>(filename, type, 800);
			break;
		}

		HealthComponent* healthComp{ enemy->AddComponent<HealthComponent>(0) };
		collisionComp->AddObserver(playerHealthComp);
		playerHealthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(healthComp);

		enemy->SetParent(parent);

		return enemy;
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
			player->BindCommand(static_cast<int>(dae::GamepadButton::X), std::make_unique<dae::BombCommand>(gameObject));

			dae::PlayerController* player1{ dae::InputManager::GetInstance().AddPlayerController(dae::PlayerController::ControlMethod::Keyboard) };
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_LEFT), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_RIGHT), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_DOWN), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_UP), std::make_unique<dae::MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_X), std::make_unique<dae::BombCommand>(gameObject));
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