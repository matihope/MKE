#pragma once
#include "MKE/Font.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"
#include <unordered_map>

namespace mk {
	class ResourceManager {
	private:
		ResourceManager() = default;
		Texture*                             getMutTexture(const ResPath& path);
		std::unordered_map<ResPath, Texture> textures;
		std::unordered_map<ResPath, Font>    fonts;

	public:
		// singleton stuff
		ResourceManager(const ResourceManager&)                   = delete;
		void                    operator=(const ResourceManager&) = delete;
		static ResourceManager& get();

		const Texture* getTexture(const ResPath& path);
		void           setTextureSmooth(const ResPath& path, bool smooth);

		const Font*    getFont(const ResPath& path);
	};
}  // namespace mk
