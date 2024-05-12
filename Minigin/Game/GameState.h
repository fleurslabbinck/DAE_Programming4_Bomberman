#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace dae
{
	using Input = int;

	class GameState
	{
	public:
		GameState() = default;
		virtual ~GameState() = default;
		GameState(const GameState& other) = delete;
		GameState(GameState&& other) = delete;
		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) = delete;

		virtual GameState* HandleGame(Input) { return nullptr; };
		virtual void OnEnter() const {};
	};
}
#endif