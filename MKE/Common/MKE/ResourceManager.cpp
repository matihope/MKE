#include "ResourceManager.hpp"

namespace mk {
	Texture* ResourceManager::getMutTexture(const ResPath& path) {
		auto&& [texture, new_texture] = textures.try_emplace(path);
		if (new_texture) texture->second.loadFromFile(path);
		return &texture->second;
	}

	const Texture* ResourceManager::getTexture(const ResPath& path) { return getMutTexture(path); }

	void ResourceManager::setTextureSmooth(const ResPath& path, bool smooth) {
		getMutTexture(path)->setSmooth(smooth);
	}

	Font* ResourceManager::getFont(const ResPath& path) {
		auto&& [font, new_font] = fonts.try_emplace(path);
		if (new_font) font->second.load(path);
		return &font->second;
	}
}  // namespace mk
