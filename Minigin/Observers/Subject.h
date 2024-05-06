#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

namespace dae
{
	using Event = int;

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
		void Notify(Event event, GameObject* gameObject) const;

	private:
		std::vector<Observer*> m_observers{};
	};
}

#endif