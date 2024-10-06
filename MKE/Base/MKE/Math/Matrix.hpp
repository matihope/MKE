#pragma once

#include "../Ints.hpp"
#include "MKE/Panic.hpp"

#include <algorithm>
#include <array>
#include <type_traits>

namespace mk::math {
	template<class T, usize H, usize W>
	requires std::is_arithmetic_v<T> && (W > 0 && H > 0) class Matrix {
	public:
		constexpr Matrix(T diagonal = 1) {
			for (usize p = 0; p < std::min(W, H); p++) operator()(p, p) = diagonal;
		}

		template<class... Args>
		requires(std::is_same_v<std::array<T, H>, Args> && ...)
		constexpr Matrix(Args&&... args): matrix{ std::forward<Args>(args)... } {}

		constexpr Matrix(const Matrix&)            = default;
		constexpr Matrix(Matrix&&)                 = default;
		constexpr Matrix& operator=(const Matrix&) = default;
		constexpr Matrix& operator=(Matrix&&)      = default;

		constexpr const T operator()(usize row, usize col) const& {
			MK_ASSERT(row < H, "Row >= H");
			MK_ASSERT(col < W, "Col >= W");
			return matrix[row][col];
		}

		constexpr T& operator()(usize row, usize col) {
			MK_ASSERT(row < H, "Row >= H");
			MK_ASSERT(col < W, "Col >= W");
			return matrix[row][col];
		}

		template<usize x1, usize x2, usize x3>
		constexpr static Matrix<T, x1, x3>
			multiply(const Matrix<T, x1, x2>& a, const Matrix<T, x2, x3>& b) {
			Matrix<T, x1, x3> result{ 0 };
			for (usize row = 0; row < x1; row++)
				for (usize col = 0; col < x3; col++)
					for (usize current = 0; current < x2; current++)
						result(row, col) += a(row, current) * b(current, col);
			return result;
		}

		constexpr const T* data() const { return matrix[0].data(); }

		friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix) {
			for (usize row = 0; row < H; row++) {
				for (usize col = 0; col < W; col++) {
					stream << matrix(row, col);
					if (col + 1 < W) stream << ' ';
				}
				stream << '\n';
			}

			return stream;
		}

	private:
		std::array<std::array<T, W>, H> matrix{};
	};

	using Matrix4f = Matrix<float, 4, 4>;
}  // namespace mk::Math
