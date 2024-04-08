#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <SDL.h>
#include <memory>
#include <string>
#include "BombermanUtil.h"
#include "Scene.h"
#include "Input/InputManager.h"
#include "Components/CollisionComponent.h"

namespace dae
{
	class GameObject;

	class Bomberman final
	{
	public:
		void LoadMainScene();

		Bomberman() = default;
		~Bomberman() = default;
		Bomberman(const Bomberman& other) = delete;
		Bomberman(Bomberman&& other) = delete;
		Bomberman& operator=(const Bomberman& other) = delete;
		Bomberman& operator=(Bomberman&& other) = delete;

	private:
		const SDL_Color m_backgroundColor{ 0, 147, 0 };
		const glm::vec2 m_fpsComponentPos{ 10.f, WINDOW_HEIGHT - (10.f * WINDOW_SCALE) };
		const float m_speed{ 40 * WINDOW_SCALE };

		void InitializePlayfield(GameObject* parent, Scene& scene, int totalCols, int totalRows) const;
		GameObject* InitializePlayer(GameObject* parent, Scene& scene, const glm::vec2& startPos, CollisionComponent::EntityType type, const glm::vec2& collisionBox, const std::string& filename) const;
		void AddControls(GameObject* gameObject, dae::PlayerController::ControlMethod controlMethod, float speed) const;

		std::unique_ptr<GameObject> GetFPSComponent(const std::string& font, int fontSize, const glm::vec2& pos) const;
	};
}
#endif