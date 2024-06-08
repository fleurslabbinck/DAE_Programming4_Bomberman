#include "BombermanManager.h"

#include "Scene/SceneManager.h"

#include <memory>
#include <fstream>
#include <ostream>
#include <json.hpp>

#include "ServiceLocator.h"
#include "Sound/SoundSystem.h"
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
#include "Components/EnterNameComponent.h"
#include "Components/ScreenComponent.h"
#include "Commands/MoveCommand.h"
#include "Commands/BombCommand.h"
#include "Commands/DetonateCommand.h"
#include "Commands/GameInputCommands.h"
#include "Commands/SkipLevelCommand.h"
#include "Commands/InfoCommand.h"
#include "Commands/ToggleMuteCommand.h"
#include "States/MainMenuState.h"
#include "States/HighScoreState.h"

namespace dae
{
	BombermanManager::~BombermanManager()
	{
		WriteHighScores();
	}

	void BombermanManager::InitializeGame()
	{
		ParseLevels();
		ParseHighScores();

		srand(static_cast<unsigned int>(time(0)));
		m_state = std::make_unique<MainMenuState>();
		m_state->OnEnter();

		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::HorizontalStep), "Sounds/HorizontalStep.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::VerticalStep), "Sounds/VerticalStep.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathSound), "Sounds/DieSound.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::DeathTune), "Sounds/BombermanDies.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::PlaceBomb), "Sounds/PlaceBomb.wav");
		ServiceLocator::GetSoundSystem().LoadSoundFX(static_cast<int>(sound::SoundId::Explosion), "Sounds/Explosion.wav");
	}

	void BombermanManager::ParseLevels()
	{
		using json = nlohmann::json;

		const std::filesystem::path filepath = m_dataLocation / "LevelSetup.json";

		std::ifstream levelSetup(filepath);

		if (!levelSetup.is_open())
		{
			std::cerr << "Could not open levelsetup\n";
			return;
		}

		json jsonData{};
		levelSetup >> jsonData;

		for (const auto& gamemode : jsonData.items())
		{
			if (gamemode.key() == "normal")
			{
				m_normalMode.playfield.columns = gamemode.value()["cols"].get<uint8_t>();
				m_normalMode.playfield.rows = gamemode.value()["rows"].get<uint8_t>();
				std::string grid{ gamemode.value()["playfield"].get<std::string>() };
				for (char c : grid) m_normalMode.playfield.grid.push_back(c);
				m_normalMode.bricks = gamemode.value()["bricks"].get<uint8_t>();

				Stage stage{};

				for (const auto& stageItem : gamemode.value()["stages"].items())
				{
					stage.ballooms = stageItem.value()["enemies"]["ballooms"].get<uint8_t>();
					stage.oneals = stageItem.value()["enemies"]["oneals"].get<uint8_t>();
					stage.dolls = stageItem.value()["enemies"]["dolls"].get<uint8_t>();
					stage.minvos = stageItem.value()["enemies"]["minvos"].get<uint8_t>();

					std::string powerUpStr = stageItem.value()["powerup"].get<std::string>();

					if (powerUpStr == "bombs") stage.powerUp = entities::EntityType::Bombs;
					else if (powerUpStr == "fire") stage.powerUp = entities::EntityType::Fire;
					else if (powerUpStr == "detonator") stage.powerUp = entities::EntityType::Detonator;

					m_normalMode.stages.push_back(stage);
				}
			}
			else if (gamemode.key() == "pvp")
			{
				m_pvpMode.playfield.columns = gamemode.value()["cols"].get<uint8_t>();
				m_pvpMode.playfield.rows = gamemode.value()["rows"].get<uint8_t>();
				std::string grid{ gamemode.value()["playfield"].get<std::string>() };
				for (char c : grid) m_pvpMode.playfield.grid.push_back(c);
				m_pvpMode.bricks = gamemode.value()["bricks"].get<uint8_t>();
			}
		}
	}

	void BombermanManager::ParseHighScores()
	{
		const std::filesystem::path filepath = m_dataLocation / m_highScoreFilepath;

		std::ifstream inputfile(filepath);

		if (!inputfile.is_open())
		{
			std::cout << "No high scores yet!\n";
			return;
		}

		HighScore highScore{};

		while (inputfile >> highScore.name >> highScore.score) m_highScores.push_back(highScore);

		inputfile.close();
	}

	void BombermanManager::WriteHighScores()
	{
		const std::filesystem::path filepath = m_dataLocation / m_highScoreFilepath;

		std::ofstream outputfile(filepath);

		if (!outputfile.is_open())
		{
			std::cout << "Failed to open outputfile" << filepath << std::endl;
			return;
		}

		for (const HighScore& highScore : m_highScores) outputfile << highScore.name << " " << highScore.score << std::endl;

		outputfile.close();
	}

	void BombermanManager::UpdateHighScores()
	{
		constexpr uint8_t maxHighScores{ 10 };

		if (m_currentHighScore.name.empty()) return;

		m_highScores.push_back(m_currentHighScore);

		std::sort(m_highScores.begin(), m_highScores.end(), [](const HighScore& highScore, const HighScore& otherHighScore) {return otherHighScore.score < highScore.score; });

		if (m_highScores.size() > maxHighScores) m_highScores.resize(maxHighScores);

		m_currentHighScore = {};
	}

	void BombermanManager::HandleGameState()
	{
		std::unique_ptr<GameState> state = m_state->HandleGameState();

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
		case scenes::Scenes::EnterName:
			LoadEnterNameScreen();
			break;
		case scenes::Scenes::StageScreen:
			LoadScreen("STAGE " + std::to_string(m_currentLevel + 1));
			break;
		case scenes::Scenes::Normal:
			m_gameMode = GameMode::Normal;
			LoadLevel();
			break;
		case scenes::Scenes::GameOverScreen:
			LoadScreen("GAME OVER");
			break;
		case scenes::Scenes::PvpScreen:
			LoadScreen("PVP");
			break;
		case scenes::Scenes::Pvp:
			m_gameMode = GameMode::Pvp;
			LoadPvp();
			break;
		case scenes::Scenes::WinnerScreen:
			LoadScreen(m_pvpWinner + " WON!");
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
		MenuControllerComponent* controllerComp{ Menu->AddComponent<MenuControllerComponent>(m_font, m_fontSize, m_textColor, m_shadowColor) };

		AddMenuControls(controllerComp, PlayerController::ControlMethod::Gamepad);
		AddMenuControls(controllerComp, PlayerController::ControlMethod::Keyboard);
	}

	void BombermanManager::LoadEnterNameScreen()
	{
		m_currentScene = "EnterName";

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		Renderer::GetInstance().SetBackgroundColor(m_stageBackgroundColor);

		GameObject* EnterName{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 0.f, 0.f)) };
		EnterNameComponent* enterNameComp{ EnterName->AddComponent<EnterNameComponent>(m_font, m_fontSize, m_textColor, m_shadowColor) };

		AddEnterNameControls(enterNameComp, PlayerController::ControlMethod::Gamepad);
		AddEnterNameControls(enterNameComp, PlayerController::ControlMethod::Keyboard);
	}

	void BombermanManager::LoadScreen(const std::string& title)
	{
		m_currentScene = title;

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		Renderer::GetInstance().SetBackgroundColor(m_stageBackgroundColor);

		GameObject* stage{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 0.f, 0.f)) };
		TextComponent* textComp{ stage->AddComponent<TextComponent>(m_font, m_fontSize, title, m_textColor, m_shadowColor) };

		const glm::vec2 textureSize{ textComp->GetRenderComponent()->GetTexture()->GetSize()};

		stage->SetPosition(constants::WINDOW_WIDTH / 2.f - textureSize.x / 2.f, constants::WINDOW_HEIGHT / 2.f - textureSize.y / 2.f);

		ScreenComponent* screenComp{ stage->AddComponent<ScreenComponent>(2.f) };
		screenComp->AddObserver(m_state.get());
	}

	void BombermanManager::LoadLevel()
	{
		m_currentScene = "Level " + std::to_string(m_currentLevel);

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* playfield{ Playfield(scene) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		if (m_totalPlayers == 2)
		{
			HealthComponent* healthComp{ player->GetComponent<HealthComponent>() };
			SecondPlayer(scene, playfield, healthComp);
		}

		for (int i{}; i < m_normalMode.stages[m_currentLevel].ballooms; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Balloom);
		for (int i{}; i < m_normalMode.stages[m_currentLevel].oneals; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Oneal);
		for (int i{}; i < m_normalMode.stages[m_currentLevel].dolls; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Doll);
		for (int i{}; i < m_normalMode.stages[m_currentLevel].minvos; ++i) Enemy(scene, playfield, scoreComp, entities::EntityType::Minvo);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");

		Renderer::GetInstance().SetBackgroundColor(m_inGameBackgroundColor);

		FPSComponent(scene);
	}

	void BombermanManager::LoadPvp()
	{
		m_currentScene = "pvp";

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		GameObject* playfield{ Playfield(scene) };

		GameObject* player{ Player(scene, playfield) };
		ScoreComponent* scoreComp{ player->GetComponent<ScoreComponent>() };

		constexpr glm::vec2 enemyPos{ 3.f * constants::GRIDCELL, 3.f * constants::GRIDCELL };

		EnemyPlayer(scene, playfield, scoreComp, enemyPos);

		ManageObservers(scene);

		// Place bricks & bomberman on top
		scene.PlaceOnTop("brick");
		scene.PlaceOnTop("player");

		Renderer::GetInstance().SetBackgroundColor(m_inGameBackgroundColor);

		FPSComponent(scene);
	}

	void BombermanManager::LoadHighScoreScene()
	{
		m_currentScene = "High Score";

		UpdateHighScores();
		ResetHealth();

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_currentScene);

		Renderer::GetInstance().SetBackgroundColor(m_stageBackgroundColor);

		GameObject* highScoreText{ scene.AddGameObject(std::make_unique<GameObject>(m_currentScene, 10.f, 10.f)) };
		highScoreText->AddComponent<TextComponent>(m_font, m_fontSize, "HIGH SCORES:", m_textColor, m_shadowColor);

		constexpr float nameStartPosX{ constants::WINDOW_WIDTH / 6.f };
		constexpr float scoreStartPosX{ 5.f * constants::WINDOW_WIDTH / 6.f };
		constexpr float startPosY{ constants::WINDOW_HEIGHT / 6.f };
		constexpr float margin{ 15.f };

		for (uint8_t idx{}; idx < m_highScores.size(); ++idx)
		{
			if (m_highScores[idx].name.empty()) continue;

			GameObject* name{ scene.AddGameObject(std::make_unique<GameObject>("name"))};
			name->AddComponent<TextComponent>(m_font, m_fontSize, m_highScores[idx].name, m_textColor, m_shadowColor);
			name->SetPosition(nameStartPosX, startPosY + idx * margin);

			GameObject* score{ scene.AddGameObject(std::make_unique<GameObject>("score"))};
			TextComponent* scoreTextComp{ score->AddComponent<TextComponent>(m_font, m_fontSize, std::format("{}", m_highScores[idx].score), m_textColor, m_shadowColor)};
			const glm::vec2 scoreTextDims{ scoreTextComp->GetRenderComponent()->GetTexture()->GetSize() };
			score->SetPosition(scoreStartPosX - scoreTextDims.x, startPosY + idx * margin);
		}

		AddNavigateControls(PlayerController::ControlMethod::Gamepad);
		AddNavigateControls(PlayerController::ControlMethod::Keyboard);
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

	GameObject* BombermanManager::Playfield(Scene& scene) const
	{
		uint8_t cols{};
		uint8_t rows{};
		std::vector<char> grid{};
		uint8_t totalBricks{};
		uint8_t totalEnemies{};

		switch (m_gameMode)
		{
		case dae::BombermanManager::GameMode::Normal:
			cols = m_normalMode.playfield.columns;
			rows = m_normalMode.playfield.rows;
			grid = m_normalMode.playfield.grid;
			totalBricks = m_normalMode.bricks;
			totalEnemies = m_normalMode.stages[m_currentLevel].ballooms + m_normalMode.stages[m_currentLevel].oneals + m_normalMode.stages[m_currentLevel].dolls + m_normalMode.stages[m_currentLevel].minvos;
			break;
		case dae::BombermanManager::GameMode::Pvp:
			cols = m_pvpMode.playfield.columns;
			rows = m_pvpMode.playfield.rows;
			grid = m_pvpMode.playfield.grid;
			totalBricks = m_pvpMode.bricks;
			totalEnemies = 1;
			break;
		}

		GameObject* playfield{ scene.AddGameObject(std::make_unique<GameObject>("playfield", 0.f, static_cast<float>(constants::WINDOW_HEIGHT - constants::GRIDCELL * rows)))};
		GridComponent* gridComp{ playfield->AddComponent<GridComponent>(cols, rows, true, m_backgroundColor) };

		const glm::vec2 blockCollisionBox{ static_cast<float>(constants::GRIDCELL), static_cast<float>(constants::GRIDCELL) };

		glm::vec2 startPos{};

		for (int idx{}; idx < grid.size(); ++idx)
		{
			if (grid[idx] == ' ') continue;
			else if (grid[idx] == 'x')
			{
				gridComp->AddFreeIdx(idx);
				continue;
			}
			else if (grid[idx] == 'o')
			{
				if (m_totalPlayers == 2) gridComp->AddFreeIdx(idx);
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

		for (int i{}; i < totalBricks; ++i) Brick(scene, playfield);

		const glm::vec2 exitPos{ Brick(scene, playfield)->GetTransform()->GetLocalPosition() };
		GameObject* exit{ scene.AddGameObject(std::make_unique<GameObject>("exit", exitPos.x, exitPos.y)) };
		exit->SetParent(playfield);
		exit->AddComponent<ExitComponent>(totalEnemies);

		std::cout << "EXIT LOCATION: " << exitPos.x / constants::GRIDCELL << ", " << exitPos.y / constants::GRIDCELL << std::endl;

		if (m_gameMode == GameMode::Normal)
		{
			const glm::vec2 powerUpPos{ Brick(scene, playfield)->GetTransform()->GetLocalPosition() };
			GameObject* powerUp{ scene.AddGameObject(std::make_unique<GameObject>("powerUp", powerUpPos.x, powerUpPos.y)) };
			powerUp->SetParent(playfield);
			powerUp->AddComponent<PowerUpComponent>(m_normalMode.stages[m_currentLevel].powerUp);

			std::cout << "POWERUP LOCATION: " << powerUpPos.x / constants::GRIDCELL << ", " << powerUpPos.y / constants::GRIDCELL << std::endl;
		}

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
		if (m_gameMode == GameMode::Normal) player->AddComponent<CameraComponent>(m_normalMode.playfield.columns * constants::GRIDCELL, 0, m_normalMode.playfield.columns * constants::GRIDCELL - constants::WINDOW_WIDTH);

		playerCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		healthComp->AddObserver(hudComp);
		spriteComp->AddObserver(healthComp);
		scoreComp->AddObserver(hudComp);

		healthComp->AddObserver(m_state.get());

		AddPlayerControls(player, PlayerController::ControlMethod::Gamepad, speed);
		//AddPlayerControls(player, PlayerController::ControlMethod::Keyboard, speed);

		return player;
	}

	GameObject* BombermanManager::SecondPlayer(Scene& scene, GameObject* parent, HealthComponent* healthComp) const
	{
		constexpr glm::vec2 startPos{ 1.f * constants::GRIDCELL, 11.f * constants::GRIDCELL };
		constexpr glm::vec2 collider{ 10.f, 14.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };

		GameObject* player{ scene.AddGameObject(std::make_unique<GameObject>("player", startPos.x, startPos.y)) };
		player->SetParent(parent);

		ColliderComponent* playerCollider{ player->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(playerCollider);

		BomberComponent* bomberComp{ player->AddComponent<BomberComponent>(scene) };
		bomberComp->SetPowerUpState(m_powerUpState);
		SpriteComponent* spriteComp{ player->AddComponent<SpriteComponent>("Sprites/Bomberman2.png", entities::EntityType::Bomberman) };

		playerCollider->AddObserver(healthComp);
		healthComp->AddObserver(spriteComp);
		spriteComp->AddObserver(healthComp);

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
		constexpr float speed{ 20.f * constants::WINDOW_SCALE };
		
		GameObject* enemy{ scene.AddGameObject(std::make_unique<GameObject>("enemyPlayer", pos.x, pos.y))};
		enemy->SetParent(parent);
		
		constexpr glm::vec2 collider{ 10.f, 12.f };
		constexpr glm::vec2 offset{ (constants::GRIDCELL - collider.x) / 2, (constants::GRIDCELL - collider.y) / 2 };

		ColliderComponent* enemyCollider{ enemy->AddComponent<ColliderComponent>(offset, collider.x, collider.y) };
		CollisionManager::GetInstance().AddCollider(enemyCollider);

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
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::BACK), std::make_unique<ToggleMuteCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_RETURN), std::move(continueCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_UP), std::move(upCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_DOWN), std::move(downCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_M), std::make_unique<ToggleMuteCommand>());
			break;
		}
		}
	}

	void BombermanManager::AddEnterNameControls(EnterNameComponent* enterNameComp, PlayerController::ControlMethod controlMethod) const
	{
		std::unique_ptr<ContinueCommand> continueCommand{ std::make_unique<ContinueCommand>() };
		continueCommand->AddObserver(enterNameComp);

		enterNameComp->AddObserver(m_state.get());

		switch (controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
		{
			PlayerController* gamepad{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Gamepad) };
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::A), std::move(continueCommand));
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::START), std::make_unique<InfoCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_RETURN), std::move(continueCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			break;
		}
		}
	}

	void BombermanManager::AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed, bool isBomberman) const
	{
		std::unique_ptr<SkipLevelCommand> skipLevelCommand{ std::make_unique<SkipLevelCommand>() };
		skipLevelCommand->AddObserver(m_state.get());

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
				gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::A), std::make_unique<BombCommand>(gameObject));
				gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::B), std::make_unique<DetonateCommand>(gameObject));
			}
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::START), std::make_unique<InfoCommand>());
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::BACK), std::make_unique<ToggleMuteCommand>());

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
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_M), std::make_unique<ToggleMuteCommand>());
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_F1),std::move(skipLevelCommand));
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_A), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ -1, 0 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_D), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 1, 0 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_S), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, 1 }));
			keyboard->BindCommand(PlayerController::KeyState::Down, static_cast<int>(SDL_SCANCODE_W), std::make_unique<MoveCommand>(gameObject, speed, glm::vec2{ 0, -1 }));
			if (isBomberman)
			{
				keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_P), std::make_unique<BombCommand>(gameObject));
				keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_O), std::make_unique<DetonateCommand>(gameObject));
			}
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_F1), std::move(skipLevelCommand));
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
			gamepad->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(GamepadButton::BACK), std::make_unique<ToggleMuteCommand>());
			break;
		}
		case PlayerController::ControlMethod::Keyboard:
		{
			PlayerController* keyboard{ InputManager::GetInstance().AddPlayerController(PlayerController::ControlMethod::Keyboard) };
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_BACKSPACE), std::move(backCommand));
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_I), std::make_unique<InfoCommand>());
			keyboard->BindCommand(PlayerController::KeyState::DownThisFrame, static_cast<int>(SDL_SCANCODE_M), std::make_unique<ToggleMuteCommand>());
			break;
		}
		}
	}
}