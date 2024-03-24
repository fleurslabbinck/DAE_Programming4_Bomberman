#include "GridComponent.h"
#include "Singletons/Renderer.h"
#include "glm/glm.hpp"

//---------------------------------
//GRIDCOMPONENT
//---------------------------------
dae::GridComponent::GridComponent(GameObject* pOwner, int cols, int rows)
	: BaseComponent(pOwner), m_cols{ cols }, m_rows{ rows }
{
	Cell cell{};

	for (int col{}; col < m_cols; ++col)
		for (int row{}; row < m_rows; ++row)
		{
			cell.startPos = { row * cell.size, col * cell.size };
			cell.center = { cell.startPos.x + cell.size / 2, cell.startPos.y + cell.size / 2 };
			m_cells.push_back(cell);
		}
}

void dae::GridComponent::Render(const glm::vec2&) const
{
	
}

glm::vec2 dae::GridComponent::GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const
{
	const int currentIdx{ PositionToIndex(currentPos) };

	if (currentIdx == -1) return currentPos;

	glm::vec2 offset{ direction * static_cast<float>(GRIDCELL) };

	return m_cells[currentIdx].center + offset;
}

int dae::GridComponent::PositionToIndex(const glm::vec2& pos) const
{
	for (size_t idx{}; idx < m_cells.size(); ++idx)
	{
		if (pos.x >= m_cells[idx].startPos.x)
			if (pos.x < m_cells[idx].startPos.x + m_cells[idx].size)
				if (pos.y >= m_cells[idx].startPos.y)
					if (pos.y < m_cells[idx].startPos.y + m_cells[idx].size)
						return static_cast<int>(idx);
	}

	return -1;
}