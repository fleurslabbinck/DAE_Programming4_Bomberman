#include "Subject.h"

namespace dae
{
	void Subject::AddObserver(Observer* observer)
	{
		m_observers.push_back(observer);
	}

	void Subject::RemoveObserver(Observer* observer)
	{
		const auto it = std::find_if(m_observers.begin(), m_observers.end(), [&](const Observer* obs) { return observer == obs; });

		m_observers.erase(it, m_observers.end());
	}

	void Subject::Notify(Event event, GameObject* gameObject) const
	{
		for (Observer* observer : m_observers)
		{
			observer->OnNotify(event, gameObject);
		}
	}
}