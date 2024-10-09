#pragma once
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"
#include <map>

namespace mk {
	class ResourceManager {
	private:
		ResourceManager() = default;
		Texture*                   getMutTexture(const ResPath& path);
		std::map<ResPath, Texture> m_textures;

	public:
		// singleton stuff
		ResourceManager(const ResourceManager&)                   = delete;
		void                    operator=(const ResourceManager&) = delete;
		static ResourceManager& get();

		const Texture* getTexture(const ResPath& path);
		void           setTextureSmooth(const ResPath& path, bool smooth);
	};
}  // namespace mk
