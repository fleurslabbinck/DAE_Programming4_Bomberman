#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H

#include "TextComponent.h"
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

		explicit FPSComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text = " ");
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;

	private:
		std::unique_ptr<TextComponent> m_textComponent;
		int m_frameCount{};
		float m_accumulatedTime{};
	};
}
#endif