#ifndef BOMBERMANMANAGER_H
#define BOMBERMANMANAGER_H

#include <iostream>
#include <filesystem>
#include <SDL.h>

#include "Singleton.h"
#include "Game/GameManager.h"
#include "Scene/Scene.h"
#include "Input/InputManager.h"
#include "BombermanUtil.h"

namespace dae
{
	class GameObject;
	class HealthComponent;
	class ScoreComponent;
	class MenuControllerComponent;
	class EnterNameComponent;

	class BombermanManager final : public Singleton<BombermanManager>, public GameManager
	{
	public:
		BombermanManager(const BombermanManager& other) = delete;
		BombermanManager(BombermanManager&& other) = delete;
		BombermanManager& operator=(const BombermanManager& other) = delete;
		BombermanManager& operator=(BombermanManager&& other) = delete;

		void SetDataLocation(const std::filesystem::path& data_location) { m_dataLocation = data_location; }
		void InitializeGame();
		void HandleGameState() override;
		void LoadScene(GameScene scene);

		void SetTotalPlayers(uint8_t totalPlayers) { m_totalPlayers = totalPlayers; }

		uint8_t GetCurrentLevel() const { return m_currentLevel; }
		uint8_t GetMaxLevels() const { return m_maxLevels; }
		void LoseHealth() { --m_currentHealth; }
		void ResetHealth() { m_currentHealth = m_maxHealth; }
		void NextLevel(powerUps::PowerUpState powerUpState);
		void ResetLevel();

		void SetPlayerName(const std::string& name) { m_currentHighScore.name = name; }
		void SetScore(int score) { m_currentHighScore.score = score; }

		void SetPvpWinner(const std::string& winner) { m_pvpWinner = winner; }

	private:
		friend class Singleton<BombermanManager>;
		BombermanManager() = default;
		~BombermanManager();

		enum class GameMode {
			Normal,
			Pvp
		};

		struct Stage {
			uint8_t ballooms;
			uint8_t oneals;
			uint8_t dolls;
			uint8_t minvos;
			entities::EntityType powerUp;
		};

		struct Playfield {
			uint8_t columns;
			uint8_t rows;
			std::vector<char> grid;
		};

		struct NormalMode {
			uint8_t bricks;
			Playfield playfield;
			std::vector<Stage> stages;
		};

		struct PvpMode {
			uint8_t bricks;
			Playfield playfield;
		};

		struct HighScore {
			std::string name{};
			int score{};
		};

		std::filesystem::path m_dataLocation{};

		// Gamemodes
		GameMode m_gameMode{ GameMode::Normal };
		NormalMode m_normalMode{};
		PvpMode m_pvpMode{};

		// Highscores
		const std::string m_highScoreFilepath{ "highscores.txt" };
		HighScore m_currentHighScore{};
		std::vector<HighScore> m_highScores{};

		// Game Info
		uint8_t m_totalPlayers{};

		const uint8_t m_maxHealth{ 2 };
		uint8_t m_currentHealth{ m_maxHealth };
		const uint8_t m_maxLevels{ 3 };
		uint8_t m_currentLevel{};

		powerUps::PowerUpState m_powerUpState{};

		std::string m_pvpWinner{};

		// Render Info
		SDL_Color m_textColor{ 255, 255, 255, 255 };
		SDL_Color m_shadowColor{ 200, 200, 200, 200 };

		SDL_Color m_inGameBackgroundColor{ 176, 176, 176 };
		SDL_Color m_stageBackgroundColor{ 0, 0, 0 };

		const int m_fontSize{ 8 };
		const std::string m_font{ "nintendo-nes-font.otf" };
		const SDL_Color m_backgroundColor{ 0, 147, 0 };

		// Scene Info
		std::string m_currentScene{};

		void ParseLevels();
		void ParseHighScores();
		void WriteHighScores();
		void UpdateHighScores();

		void LoadMenuScene();
		void LoadEnterNameScreen();
		void LoadScreen(const std::string& title);
		void LoadLevel();
		void LoadPvp();
		void LoadHighScoreScene();

		void ManageObservers(Scene& scene) const;

		GameObject* Playfield(Scene& scene) const;
		GameObject* Brick(Scene& scene, GameObject* parent) const;
		GameObject* Player(Scene& scene, GameObject* parent) const;
		GameObject* SecondPlayer(Scene& scene, GameObject* parent, HealthComponent* healthComp) const;
		GameObject* Enemy(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, entities::EntityType enemyType) const;
		GameObject* EnemyPlayer(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, const glm::vec2& pos) const;
		GameObject* FPSComponent(Scene& scene) const;

		void AddMenuControls(MenuControllerComponent* controllerComp, PlayerController::ControlMethod controlMethod) const;
		void AddEnterNameControls(EnterNameComponent* enterNameComp, PlayerController::ControlMethod controlMethod) const;
		void AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed, bool isBomberman = true) const;
		void AddNavigateControls(PlayerController::ControlMethod controlMethod) const;
	};
}
#endif