#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Objects/CollisionManager.h"
#include "../BombermanUtil.h"

namespace dae
{
	class GameObject;
	class GridComponent;
	class ScoreComponent;
	class ColliderComponent;
	class HealthComponent;
	class SpriteComponent;

	//---------------------------------
	//ENEMYCOMPONENT
	//---------------------------------
	class EnemyComponent final : public BaseComponent
	{
	public:
		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void Move();

		void Killed();
		int GetScore() const;

		HealthComponent* GetHealthComponent() { return m_healthComponent.get(); }

		explicit EnemyComponent(GameObject* pOwner, ScoreComponent* scoreComp, entities::EntityType enemyType, glm::vec2 collider, glm::vec2 offset, bool playerControlled = false);
		~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;

	private:
		bool m_playerControlled;
		bool m_killed{ false };
		int m_turnChance{};
		const int m_maxChance{ 10 };
		float m_speed{};
		entities::EntityType m_EntityType;
		glm::vec2 m_direction{};
		glm::vec2 m_targetPos{};
		std::vector<glm::vec2> m_directions{};

		CollisionManager& m_collisionManager{ CollisionManager::GetInstance() };

		GridComponent* m_gridComponent{ nullptr };
		std::unique_ptr<ColliderComponent> m_colliderComponent;
		std::unique_ptr<HealthComponent> m_healthComponent;
		std::unique_ptr<SpriteComponent> m_spriteComponent{};
		std::unique_ptr<ScoreComponent> m_scoreComponent{};

		bool ShouldTurn() const;
		void SetDirection();
		glm::vec2 GetRandomDirection(const std::vector<glm::vec2>& values) const;
		glm::vec2 GetDirectionToPlayer(const std::vector<glm::vec2>& values) const;
	};
}
#endif