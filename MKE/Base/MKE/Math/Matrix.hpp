#pragma once

#include "../Ints.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Panic.hpp"

#include <algorithm>
#include <array>
#include <cstring>
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

		constexpr T& operator()(usize row, usize col) & {
			MK_ASSERT(row < H, "Row >= H");
			MK_ASSERT(col < W, "Col >= W");
			return matrix[row][col];
		}

		constexpr Matrix& operator*=(const Matrix<T, W, H>& rhs) {
			Matrix result(0);
			for (usize row = 0; row < H; row++)
				for (usize col = 0; col < H; col++)
					for (usize current = 0; current < W; current++)
						result(row, col) += operator()(row, current) * rhs(current, col);
			*this = result;
			return *this;
		}

		template<template<class, usize> class DATA>
		constexpr auto operator*(const Vector<DATA, T, W>& rhs) -> Vector<DATA, T, W> {
			Vector<DATA, T, W> result(0);
			for (usize row = 0; row < H; row++)
				for (usize col = 0; col < W; col++)
					result.vec_data[row] += rhs.vec_data[col] * operator()(row, col);
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

	template<class T, usize H, usize W, usize x3>
	constexpr Matrix<T, H, x3> operator*(Matrix<T, H, W> lhs, const Matrix<T, W, x3>& rhs) {
		return lhs *= rhs;
	}

	using Matrix4f = Matrix<float, 4, 4>;

	// This is bonkers, but really handy - allow to multiply the transform matrix by ANY vector.
	template<template<class, usize> class DATA, class T, usize VW>
	constexpr Vector<DATA, T, VW> operator^(Matrix4f lhs, const Vector<DATA, T, VW>& rhs) {
		Vector4f rhs_better{ 0 };
		rhs_better.w = 1;
		std::memcpy(rhs_better.vec_data.data(), rhs.vec_data.data(), VW * sizeof(T));
		Vector<DATA, T, VW> result(0);
		for (usize row = 0; row < VW; row++)
			for (usize col = 0; col < 4; col++)
				result.vec_data.at(row) += rhs_better.vec_data[col] * lhs(row, col);
		return result;
	}
}  // namespace mk::Math
