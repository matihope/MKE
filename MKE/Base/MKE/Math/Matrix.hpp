#pragma once

#include "../Ints.hpp"
#include "MKE/Panic.hpp"

#include <algorithm>
#include <array>
#include <initializer_list>
#include <type_traits>

namespace mk::math {
	template<class T, usize H, usize W>
	requires std::is_arithmetic_v<T> class Matrix {
	public:
		Matrix() {
			for (usize p = 0; p < std::min(W, H); p++) operator()(p, p) = 1;
		}

		template<class... Args>
		requires(std::is_same_v<std::array<T, H>, Args> && ...)
		constexpr Matrix(Args&&... args): matrix{ std::forward<Args>(args)... } {}

		constexpr Matrix(const Matrix&)            = default;
		constexpr Matrix(Matrix&&)                 = default;
		constexpr Matrix& operator=(const Matrix&) = default;
		constexpr Matrix& operator=(Matrix&&)      = default;

		constexpr const T& operator()(usize row, usize col) const& {
			MK_ASSERT(row < H, "Row >= H");
			MK_ASSERT(col < W, "Col >= W");
			return matrix[row][col];
		}

		T& operator()(usize row, usize col) { return matrix[row][col]; }

		template<usize x1, usize x2, usize x3>
		static Matrix<T, x1, x3> multiply(const Matrix<T, x1, x2>& a, const Matrix<T, x2, x3>& b) {
			Matrix<T, x1, x3> result{};
			for (usize i = 0; i < x1; i++)
				for (usize j = 0; j < x3; j++)
					for (usize current = 0; current < x2; current++)
						result(i, j) += a(i, current) * b(current, j);
			return result;
		}

		constexpr const T* data() const { return matrix[0].data(); }


	private:
		std::array<std::array<T, W>, H> matrix{};
	};

	using Matrix4f = Matrix<float, 4, 4>;
}  // namespace mk::Math
