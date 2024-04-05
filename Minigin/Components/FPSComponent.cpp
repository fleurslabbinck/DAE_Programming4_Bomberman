#include "Components/FPSComponent.h"
#include "GameObject.h"

namespace dae
{
	//---------------------------------
	//FPSCOMPONENT
	//---------------------------------
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

	void FPSComponent::Initialize(const std::string& fontPath, int fontSize, const std::string& text)
	{
		m_textComponent->Initialize(fontPath, fontSize, text);
	}
}