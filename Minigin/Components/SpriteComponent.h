#ifndef	SPRITECOMPONENT_H
#define	SPRITECOMPONENT_H

#include "Components/BaseComponent.h"
#include "Components/RenderComponent.h"

namespace dae
{
	class SpriteComponent final : public BaseComponent
	{
	public:
		struct FrameIndex {
			int colIdx, rowIdx;
		};

		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void SetDirection(const glm::vec2& direction);
		void AnimateMovement();
		void AnimateDying();

		explicit SpriteComponent(GameObject* pOwner, const std::string& filename, int cols, int rows, int movementFrames, int deathFrames);
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
	protected:
		std::unique_ptr<RenderComponent> m_renderComponent;

		const int m_cols;
		const int m_rows;
		const int m_movementFrames;
		const int m_deathFrames;
		const int m_framesPerSecond{ 5 };

		FrameIndex m_startFrameLeft{}, m_startFrameRight{}, m_startFrameDown{}, m_startFrameUp{};

		FrameIndex m_startFrameIndex{};
		int m_currentIndex{};
		float m_accumulatedTime{};
	};
}

#endif