#pragma once
#include "MKE/Font.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"
#include <unordered_map>

namespace mk {
	class ResourceManager {
	public:
		const Texture* getTexture(const ResPath& path);
		void           setTextureSmooth(const ResPath& path, bool smooth);

		Font* getFont(const ResPath& path);
	private:
		Texture*                             getMutTexture(const ResPath& path);
		std::unordered_map<ResPath, Texture> textures;
		std::unordered_map<ResPath, Font>    fonts;
	};
}  // namespace mk
