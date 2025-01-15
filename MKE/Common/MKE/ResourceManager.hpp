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
		void           setTextureWrapMethod(const ResPath& path, GLint wrapMethod);

		Font* getFont(const ResPath& path);

	private:
		Texture*                                              getMutTexture(const ResPath& path);
		std::unordered_map<ResPath, std::unique_ptr<Texture>> textures;
		std::unordered_map<ResPath, std::unique_ptr<Font>>    fonts;
	};
}  // namespace mk
