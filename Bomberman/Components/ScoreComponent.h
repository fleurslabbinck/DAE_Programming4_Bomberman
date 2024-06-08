#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Objects/Components/BaseComponent.h"
#include "Observers/Subject.h"
#include "../BombermanUtil.h"

namespace dae
{
	using Event = int;

	class GameObject;

	//---------------------------------
	//SCORECOMPONENT
	//---------------------------------
	class ScoreComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		void OnNotify(Event event, GameObject* gameObject) override;

		int GetScore() const { return m_score; }
		int GetTotalScore() const { return m_totalScore; }

		explicit ScoreComponent(GameObject* pOwner, int totalScore, int enemyScore = 0);
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;

	private:
		int m_score{};
		int m_totalScore{};
	};
}
#endif