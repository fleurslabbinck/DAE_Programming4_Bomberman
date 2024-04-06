#include "GridComponent.h"
#include "GameObject.h"
#include "glm/glm.hpp"

namespace dae
{
	//---------------------------------
	//GRIDCOMPONENT
	//---------------------------------
	GridComponent::GridComponent(GameObject* pOwner, int cols, int rows)
		: BaseComponent(pOwner), m_cols{ cols }, m_rows{ rows }
	{
		Cell cell{};

		//const glm::vec2 gridStartPos{ pOwner->GetTransform()->GetLocalPosition() };

		for (int row{}; row < m_rows; ++row)
			for (int col{}; col < m_cols; ++col)
			{
				cell.startPos.x = cell.size * col;
				cell.startPos.y = cell.size * row;
				cell.center = { cell.startPos.x + cell.size / 2, cell.startPos.y + cell.size / 2 };
				m_cells.push_back(cell);
			}
	}

	glm::vec2 GridComponent::GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const
	{
		const int currentIdx{ PositionToIndex(currentPos) };

		if (currentIdx == -1) return currentPos;

		glm::vec2 offset{ direction * static_cast<float>(GRIDCELL) };

		return m_cells[currentIdx].center + offset;
	}

	std::vector<GameObject*> GridComponent::GetEntitiesClose(const glm::vec2& pos) const
	{
		const GameObject* owner{ GetOwner() };

		std::vector<GameObject*> children{ GetChildren(owner) };
		std::vector<int> indices{ GetSurroundingIndices(PositionToIndex(pos)) };

		std::vector<GameObject*> entities{};

		for (GameObject* child : children)
		{
			const glm::vec2 childPos{ child->GetTransform()->GetLocalPosition()};

			for (int i : indices) if (PositionToIndex(childPos) == i) entities.push_back(child);
		}

		return entities;
	}

	std::vector<GameObject*> GridComponent::GetEntitiesInCell(const glm::vec2& pos) const
	{
		const GameObject* owner{ GetOwner() };

		std::vector<GameObject*> children{ GetChildren(owner) };
		const int idx{ PositionToIndex(pos) };

		std::vector<GameObject*> entities{};

		for (GameObject* child : children)
		{
			const glm::vec2 childPos{ child->GetTransform()->GetLocalPosition() };

			if (PositionToIndex(childPos) == idx) entities.push_back(child);
		}

		return entities;
	}

	int GridComponent::PositionToIndex(const glm::vec2& pos) const
	{
		const glm::vec2 position{ pos - m_cells[0].startPos };

		const int col{ static_cast<int>(position.x) / GRIDCELL };
		const int row{ static_cast<int>(position.y) / GRIDCELL };

		const int idx{ row * GRID_COLS + col };

		return idx;
	}

	std::vector<int> GridComponent::GetSurroundingIndices(int idx) const
	{
		const std::vector<int> indices{
			idx + m_cols - 1,	idx + m_cols,		idx + m_cols + 1,
			idx - 1,			idx,				idx + 1,
			idx - m_cols - 1,	idx - m_cols,		idx - m_cols + 1,
		};

		return indices;
	}

	std::vector<GameObject*> GridComponent::GetChildren(const GameObject* parent) const
	{
		const std::vector<GameObject*> children{ parent->GetChildren() };

		return children;
	}
}