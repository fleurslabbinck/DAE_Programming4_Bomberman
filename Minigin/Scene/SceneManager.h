#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "../Singleton.h"
#include "Scene.h"

namespace dae
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		enum class GameState {
			START,
			GAME,
			PAUSE,
			GAME_OVER,
		};

		Scene& CreateScene(const std::string& name);

		GameState GetGameState() const { return m_gameState; }
		void SetGameState(GameState newGameState) { m_gameState = newGameState; }

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes;

		GameState m_gameState{ GameState::GAME };
	};
}
#endif