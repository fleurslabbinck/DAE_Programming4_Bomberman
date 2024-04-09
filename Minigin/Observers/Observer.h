#ifndef OBSERVER_H
#define OBSERVER_H

namespace dae
{
	enum class GameEvent {
		PLAYER_HIT,
		ENEMY_HIT,
		WALL_HIT,
		PLAYER_DEATH,
		ENEMY_DEATH,
		WALL_DEATH,	
		HEALTH_CHANGED,
		SCORE_CHANGED,
		PLAYER_RESPAWN,
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