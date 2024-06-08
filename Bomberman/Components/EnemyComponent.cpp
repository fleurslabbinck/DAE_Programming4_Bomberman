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
	EnemyComponent::EnemyComponent(GameObject* pOwner, ScoreComponent* scoreComp, entities::EntityType enemyType, glm::vec2 collider, glm::vec2 offset, bool playerControlled)
		: BaseComponent(pOwner), m_playerControlled{ playerControlled }, m_EntityType{ enemyType },
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
			m_turnChance = 3;
			m_speed = 15;
			break;
		case entities::EntityType::Doll:
			filepath = "Sprites/Doll.png";
			score = 400;
			m_turnChance = 3;
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
		m_scoreComponent = std::make_unique<ScoreComponent>(pOwner, 0, score);

		m_collisionManager.AddCollider(m_colliderComponent.get());

		m_colliderComponent->AddObserver(m_healthComponent.get());
		m_healthComponent->AddObserver(m_spriteComponent.get());
		m_spriteComponent->AddObserver(m_healthComponent.get());
		m_spriteComponent->AddObserver(scoreComp);

		m_gridComponent = pOwner->GetParent()->GetComponent<GridComponent>();

		m_targetPos = m_colliderComponent->GetLocalCenter();

		m_directions.push_back({ -1, 0 });
		m_directions.push_back({ 1, 0 });
		m_directions.push_back({ 0, -1 });
		m_directions.push_back({ 0, 1 });

		SetDirection();
	}

	void EnemyComponent::Update()
	{
		m_spriteComponent->Update();
		if (!m_playerControlled) Move();
	}

	void EnemyComponent::Render(const glm::vec2& pos) const
	{
		m_spriteComponent->Render(pos);
	}

	void EnemyComponent::Move()
	{
		if (m_spriteComponent->IsDead()) return;

		glm::vec2 currentPos{ m_colliderComponent->GetLocalCenter() };

		if ((m_direction == glm::vec2{ -1.f, 0 } && currentPos.x <= m_targetPos.x) ||
			(m_direction == glm::vec2{ 1.f, 0 } && currentPos.x >= m_targetPos.x) ||
			(m_direction == glm::vec2{ 0, 1.f } && currentPos.y >= m_targetPos.y) ||
			(m_direction == glm::vec2{ 0, -1.f } && currentPos.y <= m_targetPos.y))
		{
			GetOwner()->GetTransform()->Translate(-(currentPos - m_targetPos));

			m_targetPos = m_gridComponent->GetNextPosition(currentPos, m_direction);

			if (!m_collisionManager.CanMove(m_colliderComponent.get(), m_targetPos) || ShouldTurn())
			{
				SetDirection();
				m_spriteComponent->SetDirection(m_direction);
			
				m_targetPos = m_gridComponent->GetNextPosition(currentPos, m_direction);
			}
		}

		// move object
		const glm::vec2 offset{ m_direction * m_speed * TimeManager::GetInstance().GetDeltaTime() };
		GetOwner()->GetTransform()->Translate(offset);
		m_spriteComponent->AnimateMovement();
	}

	bool EnemyComponent::ShouldTurn() const
	{
		const int randNr{ rand() % m_maxChance };
		bool shouldTurn{ false };

		if (randNr < m_turnChance) shouldTurn = true;

		return shouldTurn;
	}

	void EnemyComponent::SetDirection()
	{
		std::vector<glm::vec2> validDirections{};

		for (const glm::vec2& direction : m_directions)
		{
			if (m_collisionManager.CanMove(m_colliderComponent.get(), m_gridComponent->GetNextPosition(m_colliderComponent->GetLocalCenter(), direction))) validDirections.push_back(direction);
		}

		if (validDirections.empty())
		{
			m_direction = {};
			return;
		}
		else if (validDirections.size() == 1)
		{
			m_direction = validDirections.front();
			return;
		}

		switch (m_EntityType)
		{
		case entities::EntityType::Balloom:
			m_direction = GetRandomDirection(validDirections);
			break;
		case entities::EntityType::Oneal:
			m_direction = GetDirectionToPlayer(validDirections);
			break;
		case entities::EntityType::Doll:
			m_direction = GetDirectionToPlayer(validDirections);
			break;
		case entities::EntityType::Minvo:
			m_direction = GetRandomDirection(validDirections);
			break;
		}
	}

	glm::vec2 EnemyComponent::GetRandomDirection(const std::vector<glm::vec2>& values) const
	{
		int randIdx = rand() % values.size();
		return values[randIdx];
	}

	glm::vec2 EnemyComponent::GetDirectionToPlayer(const std::vector<glm::vec2>& values) const
	{
		const GameObject* parent{ GetOwner()->GetParent() };
		const GameObject* player{ parent->GetChildrenWithName("player").front() };
		const glm::vec2 playerPos{ player->GetTransform()->GetLocalPosition() };

		float smallestDistSqr{ FLT_MAX };
		glm::vec2 direction{};
		glm::vec2 centeredPos{ m_colliderComponent->GetLocalCenter() };

		for (const glm::vec2& dir : values)
		{
			const glm::vec2 nextPos{ centeredPos + m_gridComponent->GetNextPosition(centeredPos, dir)};
			const glm::vec2 delta{ playerPos - nextPos};
			float distSqr{ glm::dot(delta.x, delta.y) };
		
			if (distSqr < smallestDistSqr)
			{
				smallestDistSqr = distSqr;
				direction = dir;
			}
		}

		return direction;
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