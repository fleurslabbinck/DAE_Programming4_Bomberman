#include "BombermanManager.h"

#include "Scene/SceneManager.h"

#include <memory>
#include "Scene/SceneManager.h"
#include "Render/Renderer.h"
#include "Objects/GameObject.h"
#include "Objects/CollisionManager.h"
#include "Objects/Components/RenderComponent.h"
#include "Objects/Components/TextComponent.h"
#include "Objects/Components/FPSComponent.h"
#include "Components/GridComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/CameraComponent.h"
#include "Components/HUDComponent.h"
#include "Components/BomberComponent.h"
#include "Commands/MoveCommand.h"
#include "Commands/BombCommand.h"
#include "Commands/GameInputCommands.h"
#include "Commands/InfoCommand.h"

namespace dae
{
	void BombermanManager::HandleGame(Input input)
	{
		GameState* state = m_state->HandleGame(input);

		if (state == nullptr) return;

		delete m_state;
		m_state = state;

		state->OnEnter();
	}

	void BombermanManager::LoadScene(GameScene scene, int level)
	{
		if (!m_currentScene.empty())
		{
			SceneManager::GetInstance().RemoveScene(m_currentScene);
			InputManager::GetInstance().SetUpdatePlayerControllersFlag();
		}

		switch (static_cast<scenes::Scenes>(scene))
		{
		case scenes::Scenes::Menu:
			LoadMenuScene();
			break;
		case scenes::Scenes::Level:
			LoadLevel(level);
			break;
		case scenes::Scenes::HighScore:
			LoadHighScoreScene();
		}
	}

	void BombermanManager::LoadMenuScene()
	{
		m_currentScene = "Menu";

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* Menu{ scene.AddGameObject(std::make_unique<GameObject>(0.f, 0.f)) };
		Menu->AddComponent<RenderComponent>("Menu.png");

		AddMenuControls(PlayerController::ControlMethod::Gamepad);
		AddMenuControls(PlayerController::ControlMethod::Keyboard);
	}

	void BombermanManager::LoadLevel(int level)
	{
		m_currentScene = "Level " + std::to_string(level);

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* playfield{ Playfield(scene, constants::GRID_COLS, constants::GRID_ROWS) };
		Player(scene, playfield);
		Enemy(scene, playfield, entities::EntityType::Balloom);
		FPSComponent(scene);
	}

	void BombermanManager::LoadHighScoreScene()
	{
		m_currentScene = "High Score";

		ResetHealth();

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* highScoreText{ scene.AddGameObject(std::make_unique<GameObject>(0.f, 0.f)) };
		highScoreText->AddComponent<TextComponent>(m_font, m_fontSize, "HIGH SCORES:");

		AddMenuControls(PlayerController::ControlMethod::Gamepad);
		AddMenuControls(PlayerController::ControlMethod::Keyboard);
	}

	GameObject* BombermanManager::Playfield(Scene& scene, int totalCols, int totalRows) const
	{
		// Playfield
		GameObject* playfield{ scene.AddGameObject(std::make_unique<GameObject>(0.f, static_cast<float>(constants::WINDOW_HEIGHT - constants::GRIDCELL * constants::GRID_ROWS))) };
		playfield->AddComponent<GridComponent>(totalCols, totalRows, true, m_backgroundColor);

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
				const int idx{ row * totalCols + col };

				if (playfieldArr[idx] != '#') continue;

				startPos = playfield->GetComponent<dae::GridComponent>()->GetCelPosAtIdx(idx);

				GameObject* block{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
				block->SetParent(playfield);
				block->AddComponent<RenderComponent>("Obstacles/Block.png");

				ColliderComponent* blockCollider{ block->AddComponent<ColliderComponent>(glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) };
				CollisionManager::GetInstance().AddCollider(blockCollider);
			}

		return playfield;
	}

	GameObject* BombermanManager::Player(Scene& scene, GameObject* parent) const
	{
		constexpr glm::vec2 startPos{ 1.f * constants::GRIDCELL, 1.f * constants::GRIDCELL };
		constexpr glm::vec2 collider{ 10.f, 14.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };

		GameObject* player{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
		player->SetParent(parent);

		ColliderComponent* playerCollider{ player->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(playerCollider);

		player->AddComponent<BomberComponent>(scene);
		SpriteComponent* spriteComp{ player->AddComponent<SpriteComponent>("Sprites/Bomberman.png", entities::EntityType::Bomberman)};
		HealthComponent* healthComp{ player->AddComponent<HealthComponent>(entities::EntityType::Bomberman, m_currentHealth) };
		HUDComponent* hudComp{ player->AddComponent<HUDComponent>(m_font, m_fontSize) };
		player->AddComponent<CameraComponent>(constants::GRID_COLS * constants::GRIDCELL, 0, constants::GRID_COLS * constants::GRIDCELL - constants::WINDOW_WIDTH);

		playerCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		healthComp->AddObserver(hudComp);
		spriteComp->AddObserver(healthComp);

		AddPlayerControls(player, PlayerController::ControlMethod::Gamepad, speed);

		return player;
	}

	GameObject* BombermanManager::Enemy(Scene& scene, GameObject* parent, entities::EntityType enemyType) const
	{
		constexpr glm::vec2 startPos{ 9.f * constants::GRIDCELL, 1.f * constants::GRIDCELL };
		constexpr glm::vec2 collider{ 10.f, 14.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };

		GameObject* enemy{ scene.AddGameObject(std::make_unique<GameObject>(startPos.x, startPos.y)) };
		enemy->SetParent(parent);

		ColliderComponent* enemyCollider{ enemy->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(enemyCollider);

		enemy->AddComponent<BomberComponent>(scene);

		SpriteComponent* spriteComp{};

		switch (enemyType)
		{
		case entities::EntityType::Balloom:
			spriteComp = enemy->AddComponent<SpriteComponent>("Sprites/Balloom.png", enemyType, 100);
			break;
		case entities::EntityType::Oneal:
			spriteComp = enemy->AddComponent<SpriteComponent>("Sprites/Oneal.png", enemyType, 200);
			break;
		case entities::EntityType::Doll:
			spriteComp = enemy->AddComponent<SpriteComponent>("Sprites/Doll.png", enemyType, 400);
			break;
		case entities::EntityType::Minvo:
			spriteComp = enemy->AddComponent<SpriteComponent>("Sprites/Minvo.png", enemyType, 800);
			break;
		}
		
		HealthComponent* healthComp{ enemy->AddComponent<HealthComponent>(enemyType, 1) };

		enemyCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(healthComp);

		AddPlayerControls(enemy, PlayerController::ControlMethod::Keyboard, speed);

		return enemy;
	}

	GameObject* BombermanManager::FPSComponent(Scene& scene) const
	{
		GameObject* fps{ scene.AddGameObject(std::make_unique<GameObject>(5.f, constants::WINDOW_HEIGHT - (12.f))) };
		fps->AddComponent<dae::FPSComponent>(m_font, m_fontSize);

		return fps;
	}

	void BombermanManager::AddMenuControls(PlayerController::ControlMethod controlMethod) const
	{
		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* player{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			player->BindCommand(static_cast<int>(GamepadButton::A), std::make_unique<ContinueCommand>());
			player->BindCommand(static_cast<int>(GamepadButton::B), std::make_unique<BackCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* player{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			player->BindCommand(static_cast<int>(SDL_SCANCODE_RETURN), std::make_unique<ContinueCommand>());
			player->BindCommand(static_cast<int>(SDL_SCANCODE_BACKSPACE), std::make_unique<BackCommand>());
			player->BindCommand(static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		}
	}

	void BombermanManager::AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed) const
	{
		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* player{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			player->BindCommand(static_cast<int>(GamepadButton::DPadLeft), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player->BindCommand(static_cast<int>(GamepadButton::DPadRight), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player->BindCommand(static_cast<int>(GamepadButton::DPadDown), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player->BindCommand(static_cast<int>(GamepadButton::DPadUp), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			player->BindCommand(static_cast<int>(GamepadButton::X), std::make_unique<BombCommand>(gameObject));

			PlayerController* player1{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_LEFT), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_RIGHT), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_DOWN), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_UP), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			player1->BindCommand(static_cast<int>(SDL_SCANCODE_X), std::make_unique<BombCommand>(gameObject));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* player{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			player->BindCommand(static_cast<int>(SDL_SCANCODE_A), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_D), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_S), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			player->BindCommand(static_cast<int>(SDL_SCANCODE_W), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			break;
		}
		}
	}
}