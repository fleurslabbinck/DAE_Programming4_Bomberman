#ifndef	CAMERACOMPONENT_H
#define	CAMERACOMPONENT_H

#include <SDL.h>
#include "../Minigin/Objects/Components/BaseComponent.h"

namespace dae
{
	class CameraComponent final : public BaseComponent
	{
	public:
		void FollowPlayer();

		explicit CameraComponent(GameObject* pOwner, int playfieldWidth, int minBorder, int maxBorder);
		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;

	private:
		SDL_Rect m_viewport{};
		int m_clampOffset{};
		const int m_minBorder;
		const int m_maxBorder;

		void ClampPlayer();
	};
}
#endif