#include "BombermanManager.h"

#include "Scene/SceneManager.h"

#include <memory>
#include <iostream>

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
#include "Components/ScoreComponent.h"
#include "Components/CameraComponent.h"
#include "Components/HUDComponent.h"
#include "Components/BomberComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/ExitComponent.h"
#include "Components/PowerUpComponent.h"
#include "Components/MenuControllerComponent.h"
#include "Components/ScreenComponent.h"
#include "Commands/MoveCommand.h"
#include "Commands/BombCommand.h"
#include "Commands/DetonateCommand.h"
#include "Commands/GameInputCommands.h"
#include "Commands/InfoCommand.h"
#include "States/MainMenuState.h"
#include "States/HighScoreState.h"

namespace dae
{
	void BombermanManager::InitializeGame()
	{
		srand(static_cast<unsigned int>(time(0)));
		m_state = std::make_unique<MainMenuState>();
		m_state->OnEnter();
	}

	void BombermanManager::HandleGame()
	{
		std::unique_ptr<GameState> state = m_state->HandleGame();

		if (state == nullptr) return;

		m_state = std::move(state);

		m_state->OnEnter();
	}

	void BombermanManager::LoadScene(GameScene scene)
	{
		if (!m_currentScene.empty())
		{
			CollisionManager::GetInstance().RemoveAllColliders();
			SceneManager::GetInstance().RemoveScene(m_currentScene);
			InputManager::GetInstance().RemovePlayerControllers();
			Renderer::GetInstance().SetViewport(SDL_Rect{ 0, 0, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT });
		}

		switch (static_cast<scenes::Scenes>(scene))
		{
		case scenes::Scenes::Menu:
			LoadMenuScene();
			break;
		case scenes::Scenes::StageScreen:
			LoadScreen("STAGE " + std::to_string(m_currentLevel + 1));
			break;
		case scenes::Scenes::SinglePlayer:
			LoadLevel();
			break;
		case scenes::Scenes::GameOverScreen:
			LoadScreen("GAME OVER");
			break;
		case scenes::Scenes::HighScore:
			LoadHighScoreScene();
		}
	}

	void BombermanManager::NextLevel(powerUps::PowerUpState powerUpState)
	{
		++m_currentLevel;
		m_powerUpState = powerUpState;
	}

	void BombermanManager::ResetLevel()
	{
		m_currentLevel = 0;
		m_powerUpState = {};
	}

	void BombermanManager::LoadMenuScene()
	{
		m_currentScene = "Menu";

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* Menu{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 0.f, 0.f)) };
		Menu->AddComponent<RenderComponent>("Menu.png");
		MenuControllerComponent* controllerComp{ Menu->AddComponent<MenuControllerComponent>(m_font, m_fontSize) };

		AddMenuControls(controllerComp, PlayerController::ControlMethod::Gamepad);
		AddMenuControls(controllerComp, PlayerController::ControlMethod::Keyboard);
	}

	void BombermanManager::LoadScreen(const std::string& title)
	{
		m_currentScene = title;

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		Renderer::GetInstance().SetBackgroundColor(m_stageBackgroundColor);

		GameObject* stage{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 0.f, 0.f)) };
		TextComponent* textComp{ stage->AddComponent<TextComponent>(m_font, m_fontSize, title) };

		const glm::vec2 textureSize{ textComp->GetRenderComponent()->GetTexture()->GetSize()};

		stage->SetPosition(constants::WINDOW_WIDTH / 2.f - textureSize.x / 2.f, constants::WINDOW_HEIGHT / 2.f - textureSize.y / 2.f);

		ScreenComponent* screenComp{ stage->AddComponent<ScreenComponent>(2.f) };
		screenComp->AddObserver(m_state.get());
	}

	void BombermanManager::LoadLevel()
	{
		m_currentScene = "Level " + std::to_string(m_currentLevel);

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		switch (m_currentLevel)
		{
		case 0:
			LoadStage1(scene);
			break;
		case 1:
			LoadStage2(scene);
			break;
		case 2:
			LoadStage3(scene);
			break;
		case 3:
			LoadStage4(scene);
			break;
		}

		Renderer::GetInstance().SetBackgroundColor(m_inGameBackgroundColor);

		FPSComponent(scene);
	}

	void BombermanManager::LoadHighScoreScene()
	{
		m_currentScene = "High Score";

		ResetHealth();

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		Renderer::GetInstance().SetBackgroundColor(m_stageBackgroundColor);

		GameObject* highScoreText{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 10.f, 10.f)) };
		highScoreText->AddComponent<TextComponent>(m_font, m_fontSize, "HIGH SCORES:");

		AddNavigateControls(PlayerController::ControlMethod::Gamepad);
		AddNavigateControls(PlayerController::ControlMethod::Keyboard);
	}

	void BombermanManager::LoadStage1(Scene& scene)
	{
		constexpr uint8_t amtBallooms{ 6 };

		GameObject* playfield{ Playfield(scene, constants::GRID_COLS, constants::GRID_ROWS, amtBallooms, entities::EntityType::Bombs) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		for (int i{}; i < amtBallooms; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Balloom);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");

		//EnemyPlayer(scene, playfield, scoreComp, glm::vec2{ 3.f * constants::GRIDCELL, 3.f * constants::GRIDCELL });
	}

	void BombermanManager::LoadStage2(Scene& scene)
	{
		constexpr uint8_t amtBallooms{ 3 };
		constexpr uint8_t amtOneals{ 3 };

		GameObject* playfield{ Playfield(scene, constants::GRID_COLS, constants::GRID_ROWS, amtBallooms + amtOneals, entities::EntityType::Fire) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		for (int i{}; i < amtBallooms; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Balloom);
		for (int i{}; i < amtOneals; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Oneal);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");
	}

	void BombermanManager::LoadStage3(Scene& scene)
	{
		constexpr uint8_t amtBallooms{ 2 };
		constexpr uint8_t amtOneals{ 2 };
		constexpr uint8_t amtDolls{ 2 };

		GameObject* playfield{ Playfield(scene, constants::GRID_COLS, constants::GRID_ROWS, amtBallooms + amtOneals + amtDolls, entities::EntityType::Detonator) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		for (int i{}; i < amtBallooms; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Balloom);
		for (int i{}; i < amtOneals; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Oneal);
		for (int i{}; i < amtDolls; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Doll);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");
	}

	void BombermanManager::LoadStage4(Scene& scene)
	{
		constexpr uint8_t amtBallooms{ 1 };
		constexpr uint8_t amtOneals{ 1 };
		constexpr uint8_t amtDolls{ 2 };
		constexpr uint8_t amtMinvos{ 2 };

		GameObject* playfield{ Playfield(scene, constants::GRID_COLS, constants::GRID_ROWS, amtBallooms + amtOneals + amtDolls + amtMinvos, entities::EntityType::Bombs) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		for (int i{}; i < amtBallooms; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Balloom);
		for (int i{}; i < amtOneals; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Oneal);
		for (int i{}; i < amtDolls; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Doll);
		for (int i{}; i < amtMinvos; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Minvo);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");
	}

	void BombermanManager::ManageObservers(Scene& scene) const
	{
		std::vector<GameObject*> objects{};

		// Player
		objects = scene.GetGameObjects("player");

		if (objects.empty()) return;

		GameObject* player{ objects.front() };

		HealthComponent* playerHealth{ player->GetComponent<HealthComponent>() };
		BomberComponent* bomberComp{ player->GetComponent<BomberComponent>() };

		// Exit
		objects = scene.GetGameObjects("exit");

		if (objects.empty()) return;

		GameObject* exit{ objects.front() };

		ExitComponent* exitComp{ exit->GetComponent<ExitComponent>() };

		// Observer to check overlap between player and exit
		playerHealth->AddObserver(exitComp);

		// Observer to check if game won
		exitComp->AddObserver(m_state.get());

		// PowerUps
		objects = scene.GetGameObjects("powerUp");

		if (!objects.empty())
		{
			// Observer to overlap between player and powerUp
			for (GameObject* powerUp : objects)
			{
				PowerUpComponent* powerUpComp{ powerUp->GetComponent<PowerUpComponent>() };
				playerHealth->AddObserver(powerUpComp);
				powerUpComp->AddObserver(bomberComp);
				powerUpComp->AddObserver(m_state.get());
			}
		}

		// Enemies
		objects = scene.GetGameObjects("enemy");

		if (!objects.empty())
		{
			// Observer to update exit door when enemy is killed
			for (GameObject* enemy : objects) enemy->GetComponent<EnemyComponent>()->GetHealthComponent()->AddObserver(exitComp);
		}

		// Enemy Player
		objects = scene.GetGameObjects("enemyPlayer");

		if (!objects.empty())
		{
			// Observer to update exit door when enemy is killed
			objects.front()->GetComponent<HealthComponent>()->AddObserver(exitComp);
		}
	}

	GameObject* BombermanManager::Playfield(Scene& scene, int totalCols, int totalRows, uint8_t totalEnemies, entities::EntityType powerUpType) const
	{
		// Playfield
		GameObject* playfield{ scene.AddGameObject(std::make_unique<GameObject>("playfield", 0.f, static_cast<float>(constants::WINDOW_HEIGHT - constants::GRIDCELL * constants::GRID_ROWS)))};
		GridComponent* gridComp{ playfield->AddComponent<GridComponent>(totalCols, totalRows, true, m_backgroundColor) };

		const char playfieldArr[constants::GRID_COLS * constants::GRID_ROWS]{
			'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
			'#', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
			'#', 'x', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
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

		for (unsigned int idx{}; idx < static_cast<unsigned int>(totalCols) * static_cast<unsigned int>(totalRows); ++idx)
		{
			if (playfieldArr[idx] == ' ') continue;
			else if (playfieldArr[idx] == 'x')
			{
				gridComp->AddFreeIdx(idx);
				continue;
			}

			startPos = gridComp->GetCelPosAtIdx(idx);
			gridComp->OccupyCell(startPos);

			GameObject* block{ scene.AddGameObject(std::make_unique<GameObject>("block", startPos.x, startPos.y)) };
			block->SetParent(playfield);
			block->AddComponent<RenderComponent>("Sprites/Block.png");

			ColliderComponent* blockCollider{ block->AddComponent<ColliderComponent>(glm::vec2{}, static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL), false) };
			CollisionManager::GetInstance().AddCollider(blockCollider);
		}

		constexpr int amtBricks{ 58 };
		for (int i{}; i < amtBricks; ++i) Brick(scene, playfield);

		const glm::vec2 exitPos{ Brick(scene, playfield)->GetTransform()->GetLocalPosition() };
		GameObject* exit{ scene.AddGameObject(std::make_unique<GameObject>("exit", exitPos.x, exitPos.y)) };
		exit->SetParent(playfield);
		exit->AddComponent<ExitComponent>(m_maxLevels, m_currentLevel, totalEnemies);

		const glm::vec2 powerUpPos{ Brick(scene, playfield)->GetTransform()->GetLocalPosition() };
		GameObject* powerUp{ scene.AddGameObject(std::make_unique<GameObject>("powerUp", powerUpPos.x, powerUpPos.y)) };
		powerUp->SetParent(playfield);
		powerUp->AddComponent<PowerUpComponent>(powerUpType);

		std::cout << "EXIT LOCATION: " << exitPos.x / constants::GRIDCELL << ", " << exitPos.y / constants::GRIDCELL << std::endl;
		std::cout << "POWERUP LOCATION: " << powerUpPos.x / constants::GRIDCELL << ", " << powerUpPos.y / constants::GRIDCELL << std::endl;

		return playfield;
	}

	GameObject* BombermanManager::Brick(Scene& scene, GameObject* parent) const
	{
		GridComponent* gridComp{ parent->GetComponent<GridComponent>() };
		const glm::vec2 gridPos{ gridComp->GetFreeCell() };

		constexpr glm::vec2 collider{ static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL) };

		GameObject* brick{ scene.AddGameObject(std::make_unique<GameObject>("brick", gridPos.x, gridPos.y))};
		brick->SetParent(parent);

		ColliderComponent* brickColliderStatic{ brick->AddComponent<ColliderComponent>(glm::vec2{}, collider.x, collider.y, false) };
		CollisionManager::GetInstance().AddCollider(brickColliderStatic);

		SpriteComponent* spriteComp{ brick->AddComponent<SpriteComponent>("Sprites/Brick.png", entities::EntityType::Brick) };
		HealthComponent* healthComp{ brick->AddComponent<HealthComponent>(entities::EntityType::Brick, 1) };

		healthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(healthComp);

		return brick;
	}

	GameObject* BombermanManager::Player(Scene& scene, GameObject* parent) const
	{
		constexpr glm::vec2 startPos{ 1.f * constants::GRIDCELL, 1.f * constants::GRIDCELL };
		constexpr glm::vec2 collider{ 10.f, 14.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };

		GameObject* player{ scene.AddGameObject(std::make_unique<GameObject>("player", startPos.x, startPos.y))};
		player->SetParent(parent);

		ColliderComponent* playerCollider{ player->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(playerCollider);

		BomberComponent* bomberComp{ player->AddComponent<BomberComponent>(scene) };
		bomberComp->SetPowerUpState(m_powerUpState);
		SpriteComponent* spriteComp{ player->AddComponent<SpriteComponent>("Sprites/Bomberman.png", entities::EntityType::Bomberman)};
		HealthComponent* healthComp{ player->AddComponent<HealthComponent>(entities::EntityType::Bomberman, m_currentHealth) };
		ScoreComponent* scoreComp{ player->AddComponent<ScoreComponent>() };
		HUDComponent* hudComp{ player->AddComponent<HUDComponent>(m_font, m_fontSize) };
		player->AddComponent<CameraComponent>(constants::GRID_COLS * constants::GRIDCELL, 0, constants::GRID_COLS * constants::GRIDCELL - constants::WINDOW_WIDTH);

		playerCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		healthComp->AddObserver(hudComp);
		spriteComp->AddObserver(healthComp);
		scoreComp->AddObserver(hudComp);

		healthComp->AddObserver(m_state.get());

		AddPlayerControls(player, PlayerController::ControlMethod::Gamepad, speed);
		AddPlayerControls(player, PlayerController::ControlMethod::Keyboard, speed);

		return player;
	}

	GameObject* BombermanManager::Enemy(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, entities::EntityType enemyType) const
	{
		GridComponent* gridComp{ parent->GetComponent<GridComponent>() };
		const glm::vec2 gridPos{ gridComp->GetFreeCell() };
		gridComp->OccupyCell(gridPos);

		GameObject* enemy{ scene.AddGameObject(std::make_unique<GameObject>("enemy", gridPos.x, gridPos.y))};
		enemy->SetParent(parent);

		constexpr glm::vec2 collider{ 10.f, 12.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };

		enemy->AddComponent<EnemyComponent>(scoreComp, enemyType, collider, offset);

		return enemy;
	}

	GameObject* BombermanManager::EnemyPlayer(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, const glm::vec2& pos) const
	{
		constexpr glm::vec2 collider{ 10.f, 14.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };

		GameObject* enemy{ scene.AddGameObject(std::make_unique<GameObject>("enemyPlayer", pos.x, pos.y))};
		enemy->SetParent(parent);

		ColliderComponent* enemyCollider{ enemy->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(enemyCollider);
		
		//enemy->AddComponent<BomberComponent>(scene);
		
		SpriteComponent* spriteComp{ enemy->AddComponent<SpriteComponent>("Sprites/Balloom.png", entities::EntityType::Balloom) };
		HealthComponent* healthComp{ enemy->AddComponent<HealthComponent>(entities::EntityType::Balloom, 1, true) };
		enemy->AddComponent<ScoreComponent>(100);
		
		enemyCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(healthComp);
		spriteComp->AddObserver(scoreComp);

		AddPlayerControls(enemy, PlayerController::ControlMethod::Keyboard, speed);

		return enemy;
	}

	GameObject* BombermanManager::FPSComponent(Scene& scene) const
	{
		GameObject* fps{ scene.AddGameObject(std::make_unique<GameObject>("fps", 5.f, constants::WINDOW_HEIGHT - (12.f))) };
		fps->AddComponent<dae::FPSComponent>(m_font, m_fontSize);

		return fps;
	}

	void BombermanManager::AddMenuControls(MenuControllerComponent* controllerComp, PlayerController::ControlMethod controlMethod) const
	{
		std::unique_ptr<ContinueCommand> continueCommand{ std::make_unique<ContinueCommand>() };
		continueCommand->AddObserver(controllerComp);

		std::unique_ptr<UpCommand> upCommand{ std::make_unique<UpCommand>() };
		upCommand->AddObserver(controllerComp);

		std::unique_ptr<DownCommand> downCommand{ std::make_unique<DownCommand>() };
		downCommand->AddObserver(controllerComp);

		controllerComp->AddObserver(m_state.get());

		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* gamepad{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::A), std::move(continueCommand));
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::DPadUp), std::move(upCommand));
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::DPadDown), std::move(downCommand));
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::START), std::make_unique<InfoCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_RETURN), std::move(continueCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_UP), std::move(upCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_DOWN), std::move(downCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		}
	}

	void BombermanManager::AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed, bool isBomberman) const
	{
		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* gamepad{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			gamepad->BindCommand(PlayerController::KeyState::Down, static_cast<int>(GamepadButton::DPadLeft), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			gamepad->BindCommand(PlayerController::KeyState::Down, static_cast<int>(GamepadButton::DPadRight), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			gamepad->BindCommand(PlayerController::KeyState::Down, static_cast<int>(GamepadButton::DPadDown), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			gamepad->BindCommand(PlayerController::KeyState::Down, static_cast<int>(GamepadButton::DPadUp), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			if (isBomberman)
			{
				gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::X), std::make_unique<BombCommand>(gameObject));
				gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::A), std::make_unique<DetonateCommand>(gameObject));
			}
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::START), std::make_unique<InfoCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_LEFT), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_RIGHT), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_DOWN), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_UP), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			if (isBomberman)
			{
				keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_X), std::make_unique<BombCommand>(gameObject));
				keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_Z), std::make_unique<DetonateCommand>(gameObject));
			}
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		}
	}

	void BombermanManager::AddNavigateControls(PlayerController::ControlMethod controlMethod) const
	{
		std::unique_ptr<BackCommand> backCommand{ std::make_unique<BackCommand>() };
		backCommand->AddObserver(m_state.get());

		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* gamepad{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::B), std::move(backCommand));
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::START), std::make_unique<InfoCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_BACKSPACE), std::move(backCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		}
	}
}