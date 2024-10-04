#pragma once

#include <cmath>
#include <initializer_list>
#include <ostream>
#include "../Ints.hpp"

namespace mk::math {

	// Make number of dimensions a parameter
	template<class T, usize SIZE>
	requires(SIZE >= 2) && std::is_arithmetic_v<T> struct Vector {
		constexpr Vector() = default;

		constexpr Vector(std::initializer_list<T> list): vec_data(list) {}

		/**
		 * @brief This is a weird constructor and it's useful if we want to cast from other types.
		 */
		template<typename K>
		constexpr explicit Vector(std::initializer_list<K> list): vec_data(list) {}

		std::array<T, SIZE> vec_data{};

		constexpr bool operator==(const Vector& rhs) const { return vec_data == rhs.data; }

		constexpr Vector& operator+=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] += rhs.data[p];
			return *this;
		}

		constexpr Vector& operator-=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] -= rhs.data[p];
			return *this;
		}

		constexpr Vector& operator*=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] *= rhs.data[p];
			return *this;
		}

		constexpr Vector& operator/=(const Vector& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] /= rhs.data[p];
			return *this;
		}

		constexpr Vector& operator*=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] *= rhs;
			return *this;
		}

		constexpr Vector& operator/=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] /= rhs;
			return *this;
		}

		constexpr Vector& operator+=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] += rhs;
			return *this;
		}

		constexpr Vector& operator-=(const auto& rhs) {
			for (usize p = 0; p < SIZE; p++) vec_data[p] -= rhs;
			return *this;
		}

		constexpr Vector operator+(const auto& rhs) const { return Vector() += rhs; }

		constexpr Vector operator-(const auto& rhs) const { return Vector() -= rhs; }

		constexpr Vector operator*(const auto& rhs) const { return Vector() *= rhs; }

		constexpr Vector operator/(const auto& rhs) const { return Vector() /= rhs; }

		T length() const { return std::sqrt(length_squared()); }

		T length_squared() const {
			T sum = 0;
			for (auto&& d: vec_data) sum += d;
			return sum / SIZE;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vector& vector) {
			for (usize p = 0; p < SIZE; p++) {
				stream << vector.data[p];
				if (p + 1 < SIZE) stream << ", ";
			}
			return stream;
		}
	};

	using Vector2f = Vector<float, 2>;
	using Vector2i = Vector<i32, 2>;
	using Vector2u = Vector<u32, 2>;

	Vector2f normalizeVector(Vector2f vector);
	Vector2f rotateVector(Vector2f vector, float angleRads);
}  // namespace mk::Math
