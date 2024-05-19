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

		void HandleGame(Input input) override;
		void LoadScene(GameScene scene, int level = 0);

		void SetState(GameState* newState) { m_state = newState; }
		void LoseHealth() { --m_currentHealth; }
		void ResetHealth() { m_currentHealth = 3; }

	private:
		friend class Singleton<BombermanManager>;
		BombermanManager() = default;
		~BombermanManager() = default;

		int m_currentHealth{ 1 };

		const int m_fontSize{ 8 };
		const std::string m_font{ "nintendo-nes-font.otf" };
		std::string m_currentScene{};
		const SDL_Color m_backgroundColor{ 0, 147, 0 };

		void LoadMenuScene();
		void LoadLevel(int level);
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