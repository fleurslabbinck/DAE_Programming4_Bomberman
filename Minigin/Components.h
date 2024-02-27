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

	//---------------------------------
	//BASE COMPONENT
	//---------------------------------
	class BaseComponent
	{
		std::weak_ptr<GameObject> m_parent;
	public:
		virtual void Update();
		virtual void Render(const glm::vec3& pos) const;
		
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		explicit BaseComponent(std::shared_ptr<GameObject> owner) : m_parent{ owner } {}
		std::weak_ptr<GameObject> GetOwner() const { return m_parent; }

		std::vector<std::shared_ptr<BaseComponent>> m_subComponents{};

		
	};

	//---------------------------------
	//TRANSFORMCOMPONENT
	//---------------------------------
	class TransformComponent : public BaseComponent
	{
	public:
		void Update() override;

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);

		explicit TransformComponent(std::shared_ptr<GameObject> owner) : BaseComponent(owner), m_position{} {}
		virtual ~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

	private:
		glm::vec3 m_position;
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

		explicit RenderComponent(std::shared_ptr<GameObject> owner) : BaseComponent(owner) {}
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

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
		void Render(const glm::vec3& pos) const;

		void Initialize(const std::string& text, std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void UpdateText();

		explicit TextComponent(std::shared_ptr<GameObject> owner) : BaseComponent(owner), m_renderComponent{ std::make_shared<RenderComponent>(owner) } { m_subComponents.push_back(m_renderComponent); }
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
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
		void Render(const glm::vec3& pos) const;

		void Initialize(std::shared_ptr<Font> font) { m_textComponent->Initialize("", font); };

		explicit FPSComponent(std::shared_ptr<GameObject> owner) : BaseComponent(owner), m_textComponent{ std::make_shared<TextComponent>(owner) } { m_subComponents.push_back(m_textComponent); }
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:
		std::shared_ptr<TextComponent> m_textComponent;

		int m_frameCount{};
		float m_accumulatedTime{};
	};
}