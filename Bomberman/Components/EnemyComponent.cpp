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
			break;
		case entities::EntityType::Oneal:
			filepath = "Sprites/Oneal.png";
			score = 200;
			break;
		case entities::EntityType::Doll:
			filepath = "Sprites/Doll.png";
			score = 400;
			break;
		case entities::EntityType::Minvo:
			filepath = "Sprites/Minvo.png";
			score = 800;
			break;
		}

		m_spriteComponent = std::make_unique<SpriteComponent>(pOwner, filepath, enemyType);
		m_scoreComponent = std::make_unique<ScoreComponent>(pOwner,score);

		m_collisionManager.AddCollider(m_colliderComponent.get());

		m_colliderComponent->AddObserver(m_healthComponent.get());
		m_healthComponent->AddObserver(m_spriteComponent.get());
		m_spriteComponent->AddObserver(m_healthComponent.get());
		m_spriteComponent->AddObserver(scoreComp);

		m_gridComponent = pOwner->GetParent()->GetComponent<GridComponent>();
	}

	EnemyComponent::~EnemyComponent()
	{
		//m_collisionManager.RemoveCollider(m_colliderComponent.get());
	}

	void EnemyComponent::Update()
	{
		m_spriteComponent->Update();
	}

	void EnemyComponent::Render(const glm::vec2& pos) const
	{
		m_spriteComponent->Render(pos);
	}

	void EnemyComponent::Move()
	{

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