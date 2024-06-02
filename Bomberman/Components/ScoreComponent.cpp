#include "ScoreComponent.h"

#include "Objects/GameObject.h"
#include "EnemyComponent.h"

namespace dae
{
	//---------------------------------
	//SCORECOMPONENT
	//---------------------------------
	ScoreComponent::ScoreComponent(GameObject* pOwner, int score)
		: BaseComponent(pOwner), m_score{ score }
	{

	}

	void ScoreComponent::OnNotify(Event event, GameObject* gameObject)
	{
		if (event == static_cast<int>(GameEvent::SCORE_CHANGED))
		{
			if (ScoreComponent* scoreComp = gameObject->GetComponent<ScoreComponent>()) m_totalScore += gameObject->GetComponent<ScoreComponent>()->GetScore();
			else m_totalScore += gameObject->GetComponent<EnemyComponent>()->GetScore();
			Notify(static_cast<int>(GameEvent::SCORE_CHANGED), GetOwner());
		}
	}
}