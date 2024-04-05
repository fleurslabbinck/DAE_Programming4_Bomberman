#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include "BaseComponent.h"
#include "MiniginUtil.h"

namespace dae
{
	class GameObject;

	//---------------------------------
	//GRIDCOMPONENT
	//---------------------------------
	class GridComponent : public BaseComponent
	{
	public:
		glm::vec2 GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const;
		std::vector<GameObject*> GetEntitiesClose(const glm::vec2& pos) const;

		explicit GridComponent(GameObject* pOwner, int cols, int rows);
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;

	private:
		const int m_cols;
		const int m_rows;
		std::vector<Cell> m_cells{};

		int PositionToIndex(const glm::vec2& pos) const;
		std::vector<int> GetSurroundingIndices(int idx) const;
		std::vector<GameObject*> GetChildren(const GameObject* parent) const;
	};
}
#endif