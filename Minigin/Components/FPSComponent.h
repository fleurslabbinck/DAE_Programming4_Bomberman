#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H

#include "Components/TextComponent.h"
#include <string>

namespace dae
{
	class GameObject;

	//---------------------------------
	//FPSCOMPONENT
	//---------------------------------
	class FPSComponent final : public BaseComponent
	{
	public:
		void Update() override;

		void Initialize(const std::string& fontPath, int fontSize, const std::string& text = " ");

		explicit FPSComponent(GameObject* pOwner) : BaseComponent(pOwner), m_textComponent{ std::make_unique<TextComponent>(pOwner) } { m_subComponents.push_back(m_textComponent.get()); }
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;

	private:
		std::unique_ptr<TextComponent> m_textComponent;
		int m_frameCount{};
		float m_accumulatedTime{};
	};
}
#endif