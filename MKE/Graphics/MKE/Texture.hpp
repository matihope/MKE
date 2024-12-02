#pragma once

#include "MKE/Image.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "glad/glad.h"

namespace mk {
	class Texture: public NonCopyable {
	public:
		Texture();
		~Texture();

		static void bind(const Texture* texture);
		static void bindAt(const Texture* texture, u32 at);

		void loadFromMemory(u32 width, u32 height, const void* data, GLenum format = GL_RGBA);
		void loadFromImage(const Image& image);
		void loadFromFile(const ResPath& file);

		void setSmooth(bool smooth);

		void generateMipmaps() const;

		math::Vector2u getSize() const;

		u32 getNativeHandle() const;

	private:
		math::Vector2u size{};
		u32            texture_id{};
		bool           is_smooth = true;
	};
}
