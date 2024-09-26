#pragma once

#include "MKE/Image.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Math/Vector2.hpp"
#include "MKE/NonCopyable.hpp"

namespace mk {
	class Texture: public NonCopyable {
	public:
		Texture();

		void use() const;

		void loadFromImage(const Image& image) const;
		void loadFromFile(const ResPath& file) const;

		void setSmooth(bool smooth) const;

		void generateMipmaps() const;

		math::Vector2u getSize() const;

		~Texture();

	private:
		math::Vector2u size{};
		u32            texture_id{};
	};
}
