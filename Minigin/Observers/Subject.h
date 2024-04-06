#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

namespace dae
{
	class Observer;

	class Subject
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

		Subject() = default;
		virtual ~Subject() = default;
		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		virtual Subject& operator=(const Subject& other) = delete;
		virtual Subject& operator=(Subject&& other) = delete;

	protected:
		void Notify(GameEvent event, const GameObject* gameObject) const;

	private:
		std::vector<Observer*> m_observers{};
	};
}

#endif