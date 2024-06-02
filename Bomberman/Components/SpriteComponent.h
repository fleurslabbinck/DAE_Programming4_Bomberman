#ifndef	SPRITECOMPONENT_H
#define	SPRITECOMPONENT_H

#include "../BombermanUtil.h"
#include "Objects/Components/BaseComponent.h"
#include "Objects/Components/RenderComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	using Event = int;

	class SpriteComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		struct FrameIndex {
			int colIdx, rowIdx;
		};

		struct Sprite {
			entities::EntityType type{ entities::EntityType::Bomberman };
			int cols{}, rows{};
			const int movementFrames{ 3 };
			int deathFrames{};
			int framesPerSecond{};
			int deathFramesPerSecond{};
			bool hasDirection{ true };
			FrameIndex startFrameLeft{}, startFrameRight{}, startFrameDown{}, startFrameUp{};
			bool enemy{ false };
		};

		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void OnNotify(Event event, GameObject* gameObject) override;

		glm::vec2 GetLastDirection() const { return m_lastDirection; }
		void SetDirection(const glm::vec2& direction);
		void AnimateMovement();
		void AnimateDying();
		void ToggleVisibility() { m_isVisible = !m_isVisible; }
		void SetDead();

		bool IsDead() const { return m_dead; };

		explicit SpriteComponent(GameObject* pOwner, const std::string& filename, entities::EntityType type);
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
	private:
		std::unique_ptr<RenderComponent> m_renderComponent;

		Sprite m_sprite{};

		glm::vec2 m_lastDirection{};

		FrameIndex m_startFrameIndex{};
		int m_currentIndex{};
		float m_accumulatedTime{};
		uint8_t m_stepCount{};
		const uint8_t m_maxStepSkip{ 3 };

		bool m_isVisible{ true };
		bool m_dead{ false };

		void PlayStepSound();
	};
}
#endif