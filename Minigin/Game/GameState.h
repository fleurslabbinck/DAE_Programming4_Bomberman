#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../Observers/Observer.h"

namespace dae
{
	class GameState : public Observer
	{
	public:
		virtual ~GameState() = default;
		virtual void OnNotify(Event event, GameObject* gameObject) override = 0;
		virtual void Update() = 0;
	};
}
#endif