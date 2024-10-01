#pragma once

#include "MKE/Ints.hpp"
#include <type_traits>

namespace mk {
	struct Color {
		float r;
		float g;
		float b;
		float a;

		// This templated constructor is to avoid ambiguity
		template<class T> requires std::is_floating_point_v<T>
		constexpr Color(T r, T g, T b, T a = 1.0): r(r), g(g), b(b), a(a) {};
		constexpr Color(u8 r, u8 g, u8 b, u8 a = 255): r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a / 255.f) {};
		constexpr Color(): Color(0, 0, 0) {};
	};

	namespace Colors {
		constexpr Color WHITE = Color(255, 255, 255);
		constexpr Color BLACK = Color(0, 0, 0);
		constexpr Color DARK  = Color(0x21, 0x21, 0x21);
	}

}
