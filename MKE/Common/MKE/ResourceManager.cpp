#include "ResourceManager.hpp"

#include <memory>

namespace mk {
	Texture* ResourceManager::getMutTexture(const ResPath& path) {
		auto&& [texture, new_texture] = textures.try_emplace(path);
		if (new_texture) {
			texture->second = std::make_unique<Texture>();
			texture->second->loadFromFile(path);
		}
		return texture->second.get();
	}

	const Texture* ResourceManager::getTexture(const ResPath& path) { return getMutTexture(path); }

	void ResourceManager::setTextureSmooth(const ResPath& path, const bool smooth) {
		getMutTexture(path)->setSmooth(smooth);
	}

	void ResourceManager::setTextureWrapMethod(const ResPath& path, const GLint wrapMethod) {
		getMutTexture(path)->setWrapMode(wrapMethod);
	}

	Font* ResourceManager::getFont(const ResPath& path) {
		auto&& [font, new_font] = fonts.try_emplace(path);
		if (new_font) {
			font->second = std::make_unique<Font>();
			font->second->load(path);
		}
		return font->second.get();
	}
}  // namespace mk
