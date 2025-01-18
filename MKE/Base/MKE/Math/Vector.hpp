#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>
#include "../Ints.hpp"
#include "MKE/Math/Base.hpp"

namespace mk::math {
	namespace impl {

		template<class T, usize SIZE>
		struct Vec;

		template<class T>
		struct Vec<T, 2> {
			union {
				struct {
					T x;
					T y;
				};

				std::array<T, 2> vec_data{};
			};

			constexpr Vec() = default;

			constexpr Vec(T value): x(value), y(value) {}

			constexpr Vec(T x, T y): x(x), y(y) {}
		};

		template<class T>
		struct Vec<T, 3> {
			union {
				struct {
					T x;
					T y;
					T z;
				};

				std::array<T, 3> vec_data{};
			};

			constexpr Vec() = default;

			constexpr Vec(T value): x(value), y(value), z(value) {}

			constexpr Vec(T x, T y, T z): x(x), y(y), z(z) {}
		};

		template<class T>
		struct Vec<T, 4> {
			union {
				struct {
					T x;
					T y;
					T z;
					T w;
				};

				std::array<T, 4> vec_data{};
			};

			constexpr Vec() = default;

			constexpr Vec(T value): x(value), y(value), z(value), w(value) {}

			constexpr Vec(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
		};
	}

	// Make number of dimensions a parameter
	template<template<class, usize> class DATA, class T, usize SIZE>
	requires(SIZE >= 2) && std::is_arithmetic_v<T> struct Vector: public DATA<T, SIZE> {
		using DATA<T, SIZE>::DATA;

		constexpr bool operator==(const Vector& rhs) const {
			return this->vec_data == rhs.vec_data;
		}

		constexpr Vector& operator+=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] += rhs.vec_data[p];
			return *this;
		}

		constexpr Vector& operator-=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] -= rhs.vec_data[p];
			return *this;
		}

		constexpr Vector& operator*=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] *= rhs.vec_data[p];
			return *this;
		}

		constexpr Vector& operator/=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] /= rhs.vec_data[p];
			return *this;
		}

		constexpr Vector& operator*=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] *= rhs;
			return *this;
		}

		constexpr Vector& operator/=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] /= rhs;
			return *this;
		}

		constexpr Vector& operator+=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] += rhs;
			return *this;
		}

		constexpr Vector& operator-=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] -= rhs;
			return *this;
		}

		constexpr Vector operator%=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) this->vec_data[p] %= rhs;
			return *this;
		}

		constexpr Vector operator+(const auto& rhs) const { return Vector(*this) += rhs; }

		constexpr Vector operator-(const auto& rhs) const { return Vector(*this) -= rhs; }

		constexpr Vector operator-() const { return Vector(*this) *= -1; }

		constexpr Vector operator*(const auto& rhs) const { return Vector(*this) *= rhs; }

		constexpr Vector operator/(const auto& rhs) const { return Vector(*this) /= rhs; }

		constexpr Vector operator%(const auto& rhs) const { return Vector(*this) %= rhs; }

		T length() const { return std::sqrt(lengthSquared()); }

		template<class K>
		constexpr auto type() const {
			Vector<DATA, K, SIZE> result;
			for (usize p = 0; p < SIZE; p++) result.vec_data[p] = K(this->vec_data[p]);
			return result;
		}

		constexpr T lengthSquared() const {
			T sum{};
			for (auto&& d: this->vec_data) sum += d * d;
			return sum;
		}

		constexpr Vector normalizeOrZero() const {
			T len = length();
			if (len < 1e-9) return *this;
			return *this / len;
		}

		constexpr friend std::ostream& operator<<(std::ostream& stream, const Vector& vector) {
			for (usize p = 0; p < SIZE; p++) {
				stream << vector.vec_data[p];
				if (p + 1 < SIZE) stream << ", ";
			}
			return stream;
		}

		constexpr Vector lerp(Vector target, float step, float ZERO = EPS_ZERO) {
			Vector res = operator*(1.f - step) + target * step;
			if ((res - target).lengthSquared() < ZERO) return target;
			return res;
		}
	};

	template<class T>
	using Vector2 = Vector<impl::Vec, T, 2>;

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<i32>;
	using Vector2u = Vector2<u32>;
	using Vector2d = Vector2<double>;

	template<class T>
	using Vector3 = Vector<impl::Vec, T, 3>;

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<i32>;
	using Vector3u = Vector3<u32>;

	template<class T>
	using Vector4 = Vector<impl::Vec, T, 4>;

	using Vector4f = Vector4<float>;
	using Vector4i = Vector4<i32>;
	using Vector4u = Vector4<u32>;
	using Vector4d = Vector4<double>;

	constexpr Vector2f normalizeVector(const Vector2f vector) {
		if (vector.x == 0 || vector.y == 0) return vector;
		float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
		return { vector.x / length, vector.y / length };
	}

	constexpr Vector2f rotateVector(Vector2f vector, float angleRads) {
		return { std::cos(angleRads) * vector.x - std::sin(angleRads) * vector.y,
			     std::sin(angleRads) * vector.x + std::cos(angleRads) * vector.y };
	}

	constexpr Vector3f cross(const Vector3f& lhs, const Vector3f& rhs) {
		return { lhs.y * rhs.z - lhs.z * rhs.y,
			     lhs.z * rhs.x - lhs.x * rhs.z,
			     lhs.x * rhs.y - lhs.y * rhs.x };
	}

	constexpr auto Vector3fZERO = Vector3f(0.f);
	constexpr auto Vector3fUP   = Vector3f(0.f, 1.f, 0.f);
	constexpr auto Vector3fDOWN = Vector3f(0.f, -1.f, 0.f);
}  // namespace mk::Math
