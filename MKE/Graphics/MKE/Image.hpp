#pragma once

#include "MKE/Ints.hpp"
#include "MKE/Math/Vector2.hpp"
#include "MKE/ResPath.hpp"

namespace mk {
	class Image {
	public:
		Image() = default;
		Image(const ResPath& image);

		~Image();
		void destroy();

		void load(const ResPath& image);
		bool tryLoad(const ResPath& image);

		const u8* getData() const;

		math::Vector2u getSize() const { return { width, height }; }

	private:
		u32 width{};
		u32 height{};
		u32 nrChannels{};
		u8* data = nullptr;
	};
}
