#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <SDL.h>
#include "Objects/Components/BaseComponent.h"
#include "../BombermanUtil.h"

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
			bool free{ true };
			float size{ static_cast<float>(constants::GRIDCELL) };
			glm::vec2 startPos;
			glm::vec2 center;
		};

		void Render(const glm::vec2& pos) const override;

		void AddFreeIdx(unsigned int idx);
		glm::vec2 GetFreeCell() const;
		void OccupyCell(const glm::vec2& pos);

		bool AreInSameCell(const glm::vec2& pos1, const glm::vec2& pos2) const { return PositionToIndex(pos1) == PositionToIndex(pos2); }
		glm::vec2 GetCelPosAtIdx(int idx) { return m_cells[idx].startPos; }
		glm::vec2 GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const;
		glm::vec2 GetGridStartPos(const glm::vec2& pos) const;
		std::vector<GameObject*> GetEntitiesInCell(const glm::vec2& pos) const;

		explicit GridComponent(GameObject* pOwner, int cols, int rows, bool setBackgroundColor = false, const SDL_Color& color = {});
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;

	private:
		const int m_cols;
		const int m_rows;
		std::vector<unsigned int> m_freeIndices{};
		std::vector<Cell> m_cells{};

		const bool m_hasBackgroundColor;
		const SDL_Color m_BackgroundColor;
		const SDL_Rect m_background;

		int PositionToIndex(const glm::vec2& pos) const;
		std::vector<GameObject*> GetChildren(const GameObject* parent) const;
	};
}
#endif