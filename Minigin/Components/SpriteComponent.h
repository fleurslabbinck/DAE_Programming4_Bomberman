#ifndef	SPRITECOMPONENT_H
#define	SPRITECOMPONENT_H

#include "Components/BaseComponent.h"
#include "Components/RenderComponent.h"
#include "Observers/Subject.h"

namespace dae
{
	class SpriteComponent final : public BaseComponent, public Observer, public Subject
	{
	public:
		enum class SpriteType {
			BOMBERMAN,
			BALLOOM,
			ONEAL,
			DOLL,
			MINVO,
			BOMB,
			EXPLOSION,
			WALL,
		};

		struct FrameIndex {
			int colIdx, rowIdx;
		};

		struct Sprite {
			SpriteType type{ SpriteType::BOMBERMAN };
			int cols{}, rows{};
			const int movementFrames{ 3 };
			int deathFrames{};
			int framesPerSecond{};
			bool hasDirection{ true };
			FrameIndex startFrameLeft{}, startFrameRight{}, startFrameDown{}, startFrameUp{};
			bool enemy{ false };
			int score{};
		};

		void Update() override;
		void Render(const glm::vec2& pos) const override;

		void OnNotify(GameEvent event, GameObject* gameObject) override;

		void SetDirection(const glm::vec2& direction);
		void AnimateMovement();
		void AnimateDying();
		void SetDead();

		bool IsDead() const { return m_dead; };
		int GetScore() const { return m_sprite.score; }

		explicit SpriteComponent(GameObject* pOwner, const std::string& filename, SpriteType type, int score = 0);
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
	private:
		std::unique_ptr<RenderComponent> m_renderComponent;

		Sprite m_sprite{};

		FrameIndex m_startFrameIndex{};
		int m_currentIndex{};
		float m_accumulatedTime{};

		bool m_dead{ false };
	};
}
#endif