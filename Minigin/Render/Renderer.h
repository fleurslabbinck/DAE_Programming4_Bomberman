#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "../Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{ 176, 176, 176 };
		SDL_Rect m_viewport{};

		int m_windowScale{};
	public:
		void Init(SDL_Window* window, int windowScale);
		void Render() const;
		void Destroy();

		SDL_Rect GetViewport() const { return { m_viewport.x / m_windowScale, m_viewport.y / m_windowScale, m_viewport.w / m_windowScale, m_viewport.h / m_windowScale }; }
		void SetViewport(const SDL_Rect& view);

		void RenderRectangle(const SDL_Rect& rect, const SDL_Color& color) const;
		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}
#endif