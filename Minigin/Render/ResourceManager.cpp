#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Resources/Texture2D.h"
#include "Resources/Font.h"

namespace dae
{
	void ResourceManager::Init(const std::filesystem::path& data)
	{
		m_dataPath = data;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}
	}

	std::unique_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file) const
	{
		const auto fullPath = m_dataPath / file;
		auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.string().c_str());
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		}
		return std::make_unique<Texture2D>(texture);
	}

	std::unique_ptr<Font> ResourceManager::LoadFont(const std::string& file, unsigned int size) const
	{
		return std::make_unique<Font>((m_dataPath / file).string(), size);
	}
}