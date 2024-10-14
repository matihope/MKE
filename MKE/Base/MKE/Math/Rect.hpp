//
// Created by mateusz on 10/2/23.
//

#pragma once

#include "Vector.hpp"
#include <type_traits>

namespace mk::math {
	template<class T>
	requires std::is_arithmetic_v<T> class Rect {
	public:
		T left, top, width, height;

		[[nodiscard]]
		math::Vector2<T> getPosition() const {
			return { left, top };
		}

		[[nodiscard]]
		math::Vector2<T> getSize() const {
			return { width, height };
		}

		constexpr T right() const { return left + width; }

		constexpr T bottom() const { return top + height; }

		template<class X>
		bool overlaps(const Rect<X>& otherRect) {
			if (left + width < otherRect.left) return false;
			if (otherRect.left + otherRect.width < left) return false;
			if (top + height < otherRect.top) return false;
			if (otherRect.top + otherRect.height < top) return false;
			return true;
		}
	};

	using RectI = Rect<int>;
	using RectF = Rect<float>;
	using RectU = Rect<unsigned int>;
}  // namespace mk
