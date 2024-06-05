#ifndef BOMBERMANMANAGER_H
#define BOMBERMANMANAGER_H

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

	class BombermanManager final : public Singleton<BombermanManager>, public GameManager
	{
	public:
		BombermanManager(const BombermanManager& other) = delete;
		BombermanManager(BombermanManager&& other) = delete;
		BombermanManager& operator=(const BombermanManager& other) = delete;
		BombermanManager& operator=(BombermanManager&& other) = delete;

		void InitializeGame();
		void HandleGame() override;
		void LoadScene(GameScene scene);

		void LoseHealth() { --m_currentHealth; }
		void ResetHealth() { m_currentHealth = m_maxHealth; }

		void NextLevel(powerUps::PowerUpState powerUpState);
		void ResetLevel();

	private:
		friend class Singleton<BombermanManager>;
		BombermanManager() = default;
		~BombermanManager() = default;

		const uint8_t m_maxHealth{ 2 };
		uint8_t m_currentHealth{ m_maxHealth };

		const uint8_t m_maxLevels{ 4 };
		uint8_t m_currentLevel{};

		powerUps::PowerUpState m_powerUpState{};

		SDL_Color m_inGameBackgroundColor{ 176, 176, 176 };
		SDL_Color m_stageBackgroundColor{ 0, 0, 0 };

		const int m_fontSize{ 8 };
		const std::string m_font{ "nintendo-nes-font.otf" };
		std::string m_currentScene{};
		const SDL_Color m_backgroundColor{ 0, 147, 0 };

		void LoadMenuScene();
		void LoadScreen(const std::string& title);
		void LoadLevel();
		void LoadHighScoreScene();

		void LoadStage1(Scene& scene);
		void LoadStage2(Scene& scene);
		void LoadStage3(Scene& scene);
		void LoadStage4(Scene& scene);

		void ManageObservers(Scene& scene) const;

		GameObject* Playfield(Scene& scene, int totalCols, int totalRows, uint8_t totalEnemies, entities::EntityType powerUpType) const;
		GameObject* Brick(Scene& scene, GameObject* parent) const;
		GameObject* Player(Scene& scene, GameObject* parent) const;
		GameObject* Enemy(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, entities::EntityType enemyType) const;
		GameObject* EnemyPlayer(Scene& scene, GameObject* parent, ScoreComponent* scoreComp, const glm::vec2& pos) const;
		GameObject* FPSComponent(Scene& scene) const;

		void AddMenuControls(PlayerController::ControlMethod controlMethod) const;
		void AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed) const;
	};
}
#endif