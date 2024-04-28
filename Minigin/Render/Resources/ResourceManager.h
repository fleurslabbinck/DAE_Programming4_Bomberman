#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <memory>
#include <filesystem>
#include "../../Singleton.h"

namespace dae
{
	class Texture2D;
	class Font;
	class SoundFX;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		std::unique_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::unique_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::unique_ptr<SoundFX> LoadSoundFX(const std::string& file) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::filesystem::path m_dataPath;
	};
}
#endif