#include "GridComponent.h"

#include "Render/Renderer.h"
#include "GameObject.h"

namespace dae
{
	//---------------------------------
	//GRIDCOMPONENT
	//---------------------------------
	GridComponent::GridComponent(GameObject* pOwner, int cols, int rows, bool setBackgroundColor, const SDL_Color& color)
		: BaseComponent(pOwner), m_cols{ cols }, m_rows{ rows }, m_hasBackgroundColor{ setBackgroundColor }, m_BackgroundColor{ color }, m_background{ 0, 0, cols * constants::GRIDCELL, rows * constants::GRIDCELL }
	{
		Cell cell{};

		for (int row{}; row < m_rows; ++row)
			for (int col{}; col < m_cols; ++col)
			{
				cell.startPos.x = cell.size * col;
				cell.startPos.y = cell.size * row;
				cell.center = { cell.startPos.x + cell.size / 2, cell.startPos.y + cell.size / 2 };
				m_cells.push_back(cell);
			}
	}

	void GridComponent::Render(const glm::vec2& pos) const
	{
		if (!m_hasBackgroundColor) return;

		SDL_Rect background{ static_cast<int>(pos.x), static_cast<int>(pos.y), m_background.w, m_background.h };

		Renderer::GetInstance().RenderRectangle(background, m_BackgroundColor);
	}

	glm::vec2 GridComponent::GetNextPosition(const glm::vec2& currentPos, const glm::vec2& direction) const
	{
		const int currentIdx{ PositionToIndex(currentPos) };

		if (currentIdx == -1) return currentPos;

		glm::vec2 offset{ direction * static_cast<float>(constants::GRIDCELL) };

		return m_cells[currentIdx].center + offset;
	}

	glm::vec2 GridComponent::GetGridStartPos(const glm::vec2& pos) const
	{
		const int currentIdx{ PositionToIndex(pos) };

		if (currentIdx == -1) return pos;

		return m_cells[currentIdx].startPos;
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

		const int col{ static_cast<int>(position.x) / constants::GRIDCELL };
		const int row{ static_cast<int>(position.y) / constants::GRIDCELL };

		const int idx{ row * constants::GRID_COLS + col };

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