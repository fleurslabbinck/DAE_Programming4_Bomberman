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

		void NextLevel() { ++m_currentLevel; }
		void ResetLevel() { m_currentLevel = 0; }
		bool GameWon() { return m_currentLevel >= m_maxLevels; }

	private:
		friend class Singleton<BombermanManager>;
		BombermanManager() = default;
		~BombermanManager() = default;

		const uint8_t m_maxHealth{ 2 };
		uint8_t m_currentHealth{ m_maxHealth };

		const uint8_t m_maxLevels{ 3 };
		uint8_t m_currentLevel{};

		SDL_Color m_inGameBackgroundColor{ 176, 176, 176 };
		SDL_Color m_stageBackgroundColor{ 0, 0, 0 };

		const int m_fontSize{ 8 };
		const std::string m_font{ "nintendo-nes-font.otf" };
		std::string m_currentScene{};
		const SDL_Color m_backgroundColor{ 0, 147, 0 };

		void LoadMenuScene();
		void LoadStageScreen();
		void LoadLevel();
		void LoadHighScoreScene();

		GameObject* Playfield(Scene& scene, int totalCols, int totalRows) const;
		GameObject* Player(Scene& scene, GameObject* parent) const;
		GameObject* Enemy(Scene& scene, GameObject* parent, entities::EntityType enemyType) const;
		GameObject* FPSComponent(Scene& scene) const;

		void AddMenuControls(PlayerController::ControlMethod controlMethod) const;
		void AddPlayerControls(GameObject* gameObject, PlayerController::ControlMethod controlMethod, float speed) const;
	};
}
#endif