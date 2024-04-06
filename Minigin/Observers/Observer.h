#ifndef OBSERVER_H
#define OBSERVER_H

namespace dae
{
	enum class GameEvent {
		HEALTH_CHANGED,
	};

	class GameObject;

	class Observer
	{
	public:
		virtual void OnNotify(GameEvent event, const GameObject* gameObject) = 0;

		Observer() = default;
		~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		virtual Observer& operator=(const Observer& other) = delete;
		virtual Observer& operator=(Observer&& other) = delete;
	};
}
#endif