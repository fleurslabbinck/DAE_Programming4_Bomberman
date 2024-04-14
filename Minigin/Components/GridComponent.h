#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <SDL.h>
#include "BaseComponent.h"
#include "BombermanUtil.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//GRIDCOMPONENT
	//---------------------------------
	class GridComponent : public BaseComponent
	{
	public:
		struct Cell {
			float size{ static_cast<float>(GRIDCELL) };
			glm::vec2 startPos;
			glm::vec2 center;
		};

		void Render(const glm::vec2& pos) const override;

		glm::vec2 GetCelPosAtIdx(int idx) { return m_cells[idx].startPos; }
		glm::vec2 GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const;
		glm::vec2 GetGridStartPos(const glm::vec2& pos) const;
		std::vector<GameObject*> GetEntitiesClose(const glm::vec2& pos) const;
		std::vector<GameObject*> GetEntitiesInCell(const glm::vec2& pos) const;

		explicit GridComponent(GameObject* pOwner, int cols, int rows, bool setBackgroundColor = false, const SDL_Color& color = {});
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;

	private:
		const int m_cols;
		const int m_rows;
		std::vector<Cell> m_cells{};

		const bool m_hasBackgroundColor;
		const SDL_Color m_BackgroundColor;
		const SDL_Rect m_background;

		int PositionToIndex(const glm::vec2& pos) const;
		std::vector<int> GetSurroundingIndices(int idx) const;
		std::vector<GameObject*> GetChildren(const GameObject* parent) const;
	};
}
#endif