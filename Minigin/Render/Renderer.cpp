#include "Renderer.h"

#include <stdexcept>
#include "../Scene/SceneManager.h"
#include "Resources/Texture2D.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

namespace dae
{
	void Renderer::Init(SDL_Window* window, int windowScale)
	{
		m_window = window;
		m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
		if (m_renderer == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
		}

		m_windowScale = windowScale;
	}

	void Renderer::Render() const
	{
		const auto& color = GetBackgroundColor();
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(m_renderer);

		SceneManager::GetInstance().Render();

		SDL_RenderPresent(m_renderer);
	}

	void Renderer::Destroy()
	{
		if (m_renderer != nullptr)
		{
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
		}
	}

	void Renderer::SetViewport(const SDL_Rect& view)
	{
		m_viewport = view;
		m_viewport.x *= m_windowScale;
		m_viewport.y *= m_windowScale;
		m_viewport.w *= m_windowScale;
		m_viewport.h *= m_windowScale;
		SDL_RenderSetViewport(m_renderer, &m_viewport);
	}

	void Renderer::RenderRectangle(const SDL_Rect& rect, const SDL_Color& color) const
	{
		SDL_Rect rectangle{ rect };
		rectangle.x *= m_windowScale;
		rectangle.y *= m_windowScale;
		rectangle.w *= m_windowScale;
		rectangle.h *= m_windowScale;
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &rectangle);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
	{
		SDL_Rect dst{};
		dst.x = static_cast<int>(x) * m_windowScale;
		dst.y = static_cast<int>(y) * m_windowScale;
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		dst.w *= m_windowScale;
		dst.h *= m_windowScale;
		SDL_RenderCopy(m_renderer, texture.GetSDLTexture(), nullptr, &dst);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
	{
		SDL_Rect dst{};
		dst.x = static_cast<int>(x) * m_windowScale;
		dst.y = static_cast<int>(y) * m_windowScale;
		dst.w = static_cast<int>(width) * m_windowScale;
		dst.h = static_cast<int>(height) * m_windowScale;
		SDL_RenderCopy(m_renderer, texture.GetSDLTexture(), nullptr, &dst);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const
	{
		SDL_Rect destRectangle{ dstRect };
		destRectangle.x *= m_windowScale;
		destRectangle.y *= m_windowScale;
		destRectangle.w *= m_windowScale;
		destRectangle.h *= m_windowScale;
		SDL_RenderCopy(m_renderer, texture.GetSDLTexture(), &srcRect, &destRectangle);
	}

	SDL_Renderer* Renderer::GetSDLRenderer() const { return m_renderer; }
}