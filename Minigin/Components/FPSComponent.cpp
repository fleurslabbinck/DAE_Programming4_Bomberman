#include "Components/FPSComponent.h"
#include "GameObject.h"

namespace dae
{
	//---------------------------------
	//FPSCOMPONENT
	//---------------------------------
	FPSComponent::FPSComponent(GameObject* pOwner, const std::string& fontPath, int fontSize, const std::string& text)
		: BaseComponent(pOwner), m_textComponent{ std::make_unique<TextComponent>(pOwner, fontPath, fontSize, text) }
	{
		m_subComponents.push_back(m_textComponent.get());
	}

	void FPSComponent::Update()
	{
		constexpr float maxTime{ 0.1f };

		TimeManager& time = TimeManager::GetInstance();

		++m_frameCount;
		m_accumulatedTime += time.GetDeltaTime();

		if (m_accumulatedTime >= maxTime)
		{
			m_textComponent->SetText(std::format("{:.1f}", m_frameCount / m_accumulatedTime));

			m_frameCount = 0;
			m_accumulatedTime = 0;
		}

		if (m_textComponent->NeedsUpdate()) m_textComponent->UpdateText();
	}
}