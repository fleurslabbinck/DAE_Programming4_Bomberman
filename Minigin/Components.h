#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	enum class ComponentType {
		TransForm,
		Render,
		Text,
		FPS
	};

	//---------------------------------
	//BASE COMPONENT
	//---------------------------------
	class BaseComponent
	{
		GameObject* m_pOwner;
	public:
		virtual void FixedUpdate();
		virtual void Update();
		virtual void Render(const glm::vec3& pos) const;

		virtual void SetDelete() { m_delete = true; }
		virtual bool ShouldBeDeleted() const { return m_delete; };
		
		virtual ~BaseComponent() = default;
		BaseComponent() = delete;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		virtual BaseComponent& operator=(const BaseComponent& other) = delete;
		virtual BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		std::vector<std::shared_ptr<BaseComponent>> m_subComponents{};

		bool m_delete{ false };

		explicit BaseComponent(GameObject* pOwner) : m_pOwner{ pOwner } {}
		GameObject* GetOwner() const { return m_pOwner; }
	};

	//---------------------------------
	//TRANSFORMCOMPONENT
	//---------------------------------
	class TransformComponent : public BaseComponent
	{
	public:
		void UpdateWorldPosition();
		void SetLocalPosition(const glm::vec3& pos);
		void SetPositionDirty();
		bool IsPositionDirty() const { return m_positionIsDirty; }
		glm::vec3 GetLocalPosition() { return m_localPosition; }
		glm::vec3 GetWorldPosition();

		explicit TransformComponent(GameObject* pOwner) : BaseComponent(pOwner) {}
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;

	private:
		glm::vec3 m_localPosition{};
		glm::vec3 m_worldPosition{};

		bool m_positionIsDirty{ true };
	};

	//---------------------------------
	//ROTATORCOMPONENT
	//---------------------------------
	class RotatorComponent : public BaseComponent
	{
	public:
		void Update() override;
		
		void SetRotation(float angle) { m_rotation = angle; };
		float GetRotation() const { return m_rotation; };
		glm::vec3 RotatePoint(const glm::vec3& pos) const;

		explicit RotatorComponent(GameObject* pOwner) : BaseComponent(pOwner) {};
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;

	private:
		float m_rotation{};
	};

	//---------------------------------
	//RENDERCOMPONENT
	//---------------------------------
	class RenderComponent : public BaseComponent
	{
	public:
		void Render(const glm::vec3& pos) const override;

		virtual void SetTexture(const std::string& filename);
		virtual void SetTexture(std::shared_ptr<Texture2D> texture);

		explicit RenderComponent(GameObject* pOwner) : BaseComponent(pOwner) {}
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_texture{};
	};

	//---------------------------------
	//TEXTCOMPONENT
	//---------------------------------
	class TextComponent : public BaseComponent
	{
	public:
		void Update() override;

		void Initialize(std::shared_ptr<Font> font, const std::string& text);
		void SetText(const std::string& text);
		void UpdateText();
		bool NeedsUpdate() const { return m_needsUpdate; };

		explicit TextComponent(GameObject* pOwner) : BaseComponent(pOwner), m_renderComponent{ std::make_shared<RenderComponent>(pOwner) } { m_subComponents.push_back(m_renderComponent); }
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
	protected:
		std::shared_ptr<RenderComponent> m_renderComponent;

		bool m_needsUpdate{ true };
		std::string m_text{};
		std::shared_ptr<Font> m_font{};
	};

	//---------------------------------
	//FPSCOMPONENT
	//---------------------------------
	class FPSComponent final : public BaseComponent
	{
	public:
		void Update() override;

		void Initialize(std::shared_ptr<Font> font, const std::string& text = " ");

		explicit FPSComponent(GameObject* pOwner) : BaseComponent(pOwner), m_textComponent{ std::make_shared<TextComponent>(pOwner) } { m_subComponents.push_back(m_textComponent); }
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;

	private:
		std::shared_ptr<TextComponent> m_textComponent;
		int m_frameCount{};
		float m_accumulatedTime{};
	};
}