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
	//BITMAPCOMPONENT
	//---------------------------------
	class BitmapComponent : public Component
	{
	public:
		virtual void Render(const Transform& transform) const override;

		BitmapComponent() = default;
		virtual ~BitmapComponent() = default;
		BitmapComponent(const BitmapComponent& other) = delete;
		BitmapComponent(BitmapComponent&& other) = delete;
		BitmapComponent& operator=(const BitmapComponent& other) = delete;
		BitmapComponent& operator=(BitmapComponent&& other) = delete;
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
		virtual void SetPosition(float x, float y) override;

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
	class FPSComponent : public Component
	{
	public:

		FPSComponent(float FPS);
		virtual ~FPSComponent();
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:

	};
}