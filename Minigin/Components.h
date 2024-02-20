#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;

	//---------------------------------
	//BASE COMPONENT
	//---------------------------------
	class Component
	{
	public:
		virtual void FixedUpdate(float deltaTime);
		virtual void Update(float deltaTime);
		virtual void Render(const Transform& transform) const;

		virtual void SetTexture(const std::string& filename);
		virtual void SetPosition(float x, float y);
		

		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	};

	//---------------------------------
	//RENDERCOMPONENT
	//---------------------------------
	class RenderComponent : public Component
	{
	public:
		virtual void Render(const Transform& transform) const override;

		RenderComponent() = default;
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	};

	//---------------------------------
	//TEXTCOMPONENT
	//---------------------------------
	class TextComponent : public Component
	{
	public:
		virtual void Update(float deltaTime) override;
		virtual void Render(const Transform& transform) const override;

		void Initialize(const std::string& text, std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void UpdateText();

		TextComponent() = default;
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		std::shared_ptr<Font> m_font{};
	};

	//---------------------------------
	//FPSCOMPONENT
	//---------------------------------
	class FPSComponent final : public TextComponent
	{
	public:
		void Update(float deltaTime) override;

		FPSComponent() = default;
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:
		int m_frameCount{};
		float m_accumulatedTime{};
	};
}