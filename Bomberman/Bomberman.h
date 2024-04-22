#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <SDL.h>
#include <memory>
#include <string>

#include "BombermanUtil.h"

#include "../Minigin/Scene/Scene.h"
#include "Input/InputManager.h"
#include "Components/CollisionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"

namespace dae
{
	class GameObject;

	class Bomberman final : public Singleton<Bomberman>
	{
	public:
		void LoadMainScene();

		Bomberman(const Bomberman& other) = delete;
		Bomberman(Bomberman&& other) = delete;
		Bomberman& operator=(const Bomberman& other) = delete;
		Bomberman& operator=(Bomberman&& other) = delete;

	private:
		friend class Singleton<Bomberman>;
		Bomberman() = default;
		~Bomberman() = default;

		const SDL_Color m_backgroundColor{ 0, 147, 0 };
		const glm::vec2 m_fpsComponentPos{ 5.f, constants::WINDOW_HEIGHT - (12.f) };
		const float m_speed{ 40 };

		void InitializePlayfield(GameObject* parent, Scene& scene, int totalCols, int totalRows) const;
		GameObject* InitializePlayer(Scene& scene, GameObject* parent, const glm::vec2& startPos, const glm::vec2& collisionBox, const std::string& filename) const;
		GameObject* InitializeEnemy(Scene& scene, GameObject* parent, const glm::vec2& startPos, const glm::vec2& collisionBox, const std::string& filename, SpriteComponent::SpriteType type, HealthComponent* playerHealthComp) const;
		void AddControls(GameObject* gameObject, dae::PlayerController::ControlMethod controlMethod, float speed) const;

		std::unique_ptr<GameObject> GetFPSComponent(const std::string& font, int fontSize, const glm::vec2& pos) const;
	};
}
#endif