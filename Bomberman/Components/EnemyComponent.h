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

		explicit EnemyComponent(GameObject* pOwner, ScoreComponent* scoreComp, entities::EntityType enemyType, glm::vec2 collider, glm::vec2 offset);
		~EnemyComponent();
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;

	private:
		bool m_killed{ false };
		entities::EntityType m_EntityType;
		glm::vec2 m_direction{};

		CollisionManager& m_collisionManager{ CollisionManager::GetInstance() };

		GridComponent* m_gridComponent{ nullptr };
		std::unique_ptr<ColliderComponent> m_colliderComponent;
		std::unique_ptr<HealthComponent> m_healthComponent;
		std::unique_ptr<SpriteComponent> m_spriteComponent{};
		std::unique_ptr<ScoreComponent> m_scoreComponent{};
	};
}
#endif