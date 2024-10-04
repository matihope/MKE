#include "ResourceManager.hpp"

namespace mk {
	ResourceManager& ResourceManager::get() {
		static ResourceManager instance;
		return instance;
	}

	Texture* ResourceManager::getMutTexture(const ResPath& path) {
		if (!m_textures.contains(path)) m_textures[path].loadFromFile(path.getPath());
		return &m_textures[path];
	}

	const Texture* ResourceManager::getTexture(const ResPath& path) { return getMutTexture(path); }

	void ResourceManager::setTextureSmooth(const ResPath& path, bool smooth) {
		getMutTexture(path)->setSmooth(smooth);
	}
}  // namespace mk
