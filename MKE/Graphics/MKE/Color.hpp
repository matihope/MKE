#pragma once

#include "MKE/Ints.hpp"

namespace mk {
	struct Color {
		u8 r;
		u8 g;
		u8 b;
		u8 a;

		constexpr Color(u8 r, u8 g, u8 b, u8 a): r(r), g(g), b(b), a(a) {};
	};

	namespace Colors {
		constexpr Color WHITE = Color(255, 255, 255, 255);
		constexpr Color BLACK = Color(0, 0, 0, 255);
		constexpr Color DARK = Color(0x21, 0x21, 0x21, 255);
	}

}
