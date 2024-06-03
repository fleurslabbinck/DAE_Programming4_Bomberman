#include "EnemyComponent.h"

#include "Objects/GameObject.h"
#include "../Components/GridComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ScoreComponent.h"

namespace dae
{
	//---------------------------------
	//ENEMYCOMPONENT
	//---------------------------------
	EnemyComponent::EnemyComponent(GameObject* pOwner, ScoreComponent* scoreComp, entities::EntityType enemyType, glm::vec2 collider, glm::vec2 offset)
		: BaseComponent(pOwner), m_EntityType{ enemyType },
		m_colliderComponent{ std::make_unique<ColliderComponent>(pOwner, offset, collider.x, collider.y) },
		m_healthComponent{ std::make_unique<HealthComponent>(pOwner, enemyType, 1) }
	{
		std::string filepath{};
		int score{};

		switch (enemyType)
		{
		case entities::EntityType::Balloom:
			filepath = "Sprites/Balloom.png";
			score = 100;
			m_turnChance = 1;
			m_speed = 10;
			break;
		case entities::EntityType::Oneal:
			filepath = "Sprites/Oneal.png";
			score = 200;
			m_turnChance = 8;
			m_speed = 15;
			break;
		case entities::EntityType::Doll:
			filepath = "Sprites/Doll.png";
			score = 400;
			m_turnChance = 8;
			m_speed = 20;
			break;
		case entities::EntityType::Minvo:
			filepath = "Sprites/Minvo.png";
			score = 800;
			m_turnChance = 1;
			m_speed = 25;
			break;
		}

		m_speed *= constants::WINDOW_SCALE;

		m_spriteComponent = std::make_unique<SpriteComponent>(pOwner, filepath, enemyType);
		m_scoreComponent = std::make_unique<ScoreComponent>(pOwner,score);

		m_collisionManager.AddCollider(m_colliderComponent.get());

		m_colliderComponent->AddObserver(m_healthComponent.get());
		m_healthComponent->AddObserver(m_spriteComponent.get());
		m_spriteComponent->AddObserver(m_healthComponent.get());
		m_spriteComponent->AddObserver(scoreComp);

		m_gridComponent = pOwner->GetParent()->GetComponent<GridComponent>();

		m_targetPos = m_colliderComponent->GetLocalCenter();
	}

	void EnemyComponent::Update()
	{
		m_spriteComponent->Update();
		Move();
	}

	void EnemyComponent::Render(const glm::vec2& pos) const
	{
		m_spriteComponent->Render(pos);
	}

	void EnemyComponent::Move()
	{
		if (m_spriteComponent->IsDead()) return;

		const glm::vec2 offset{ m_direction * m_speed * TimeManager::GetInstance().GetDeltaTime() };

		glm::vec2 centeredPos{ m_colliderComponent->GetLocalCenter() };
		const float distToTarget{ glm::distance(centeredPos, m_targetPos) };
		constexpr float targetOffset{ 0.5f };

		if (distToTarget <= targetOffset)
		{
			m_targetPos = m_gridComponent->GetNextPosition(centeredPos, m_direction);

			if (!m_collisionManager.CanMove(m_colliderComponent.get(), m_targetPos) || Turn())
			{
				m_direction = GetRandomDirection(centeredPos);
				m_spriteComponent->SetDirection(m_direction);
			}
		}

		// move object
		GetOwner()->GetTransform()->Translate(offset);
		m_spriteComponent->AnimateMovement();
	}

	glm::vec2 EnemyComponent::GetRandomDirection(const glm::vec2& centeredPos)
	{
		constexpr int amtDirections{ 4 };
		int randNr{};
		glm::vec2 direction{};

		do {
			if (IsBoxed(centeredPos)) break;

			randNr = rand() % amtDirections;

			switch (randNr)
			{
			case 0:
				direction = {-1, 0};
				break;
			case 1:
				direction = { 1, 0 };
				break;
			case 2:
				direction = { 0, -1 };
				break;
			case 3:
				direction = { 0, 1 };
				break;
			}

			m_targetPos = m_gridComponent->GetNextPosition(centeredPos, direction);

		} while (!m_collisionManager.CanMove(m_colliderComponent.get(), m_targetPos) || direction == m_direction);

		return direction;
	}

	bool EnemyComponent::Turn() const
	{
		const int randNr{ rand() % m_maxChance };
		bool shouldTurn{ false };

		if (randNr < m_turnChance) shouldTurn = true;

		return shouldTurn;
	}

	bool EnemyComponent::IsBoxed(const glm::vec2& centeredPos) const
	{
		bool boxed{ !m_collisionManager.CanMove(m_colliderComponent.get(), m_gridComponent->GetNextPosition(centeredPos, { -1, 0 })) &&
					!m_collisionManager.CanMove(m_colliderComponent.get(), m_gridComponent->GetNextPosition(centeredPos, { 1, 0 })) &&
					!m_collisionManager.CanMove(m_colliderComponent.get(), m_gridComponent->GetNextPosition(centeredPos, { 0, -1 })) &&
					!m_collisionManager.CanMove(m_colliderComponent.get(), m_gridComponent->GetNextPosition(centeredPos, { 0, 1 })) };

		return boxed;
	}

	void EnemyComponent::Killed()
	{
		m_killed = true;
		m_collisionManager.RemoveCollider(m_colliderComponent.get());
	}

	int EnemyComponent::GetScore() const
	{
		return m_scoreComponent->GetScore();
	}
}