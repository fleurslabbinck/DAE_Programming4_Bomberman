#ifndef OBSERVER_H
#define OBSERVER_H

namespace dae
{
	enum class GameEvent {
		WALL_DESTROYED,
		ENEMY_KILLED,
		PLAYER_DIED,
		HEALTH_CHANGED,
		SCORE_CHANGED,
		GAME_OVER,
	};

	class GameObject;

	class Observer
	{
	public:
		virtual void OnNotify(GameEvent event, GameObject* gameObject) = 0;

		Observer() = default;
		~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		virtual Observer& operator=(const Observer& other) = delete;
		virtual Observer& operator=(Observer&& other) = delete;
	};
}
#endif