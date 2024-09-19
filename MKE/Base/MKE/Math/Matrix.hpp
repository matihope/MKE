#pragma once

#include "Base.hpp"
#include "../Ints.hpp"

#include <array>

namespace mk::math {
	template<u32 h, u32 w>
	class Matrix: public std::array<std::array<double, w>, h> {
	public:
		using std::array<std::array<double, w>, h>::array;

		template<u32 x1, u32 x2, u32 x3>
		static Matrix<x1, x3> multiply(const Matrix<x1, x2>& a, const Matrix<x2, x3>& b) {
			Matrix<x1, x3> result{};
			for (int i = 0; i < x1; i++)
				for (int j = 0; j < x3; j++)
					for (int current = 0; current < x2; current++)
						result[i][j] += a[i][current] * b[current][j];
			return result;
		}
	};
}  // namespace mk::Math
