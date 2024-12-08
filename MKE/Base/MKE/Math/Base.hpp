//
// Created by mateusz on 9/16/23.
//

#pragma once

#include <cmath>
#include <type_traits>

#define FLOAT_INFINITY std::numeric_limits<float>::infinity()
#define INT_INFINITY   1'000'000'000
#define LL_INFINITY    1'000'000'000'000'000'000LL
#define EPS_ZERO       1e-12
#define M_PIf          3.14159265358979323846f /* pi */

namespace mk::math {
	template<typename T>
	constexpr int sign(T val) {
		return (0 < val) - (val < 0);
	}

	template<typename T>
	constexpr bool isZero(T value, T EPSILON = EPS_ZERO) {
		return std::abs(value) <= EPSILON;
	}

	/**
	 * @brief Does while(value > range) value -= range
	 * in an optimal way.
	 */
	constexpr float makeInRange(float value, float range) {
		float res = std::fmod(value, range);
		if (res > 0) return res;
		return res + value;
	}

	constexpr float radiansToDegrees(float radians) {
		return makeInRange(radians, 2 * M_PIf) / M_PIf * 180.f;
	}

	constexpr float degreesToRadians(float degrees) {
		return makeInRange(degrees, 360.f) / 180.f * M_PIf;
	}

	constexpr float lerp(float base, float target, float coeff) {
		float res = base * (1.0 - coeff) + target * coeff;
		if (std::abs(res - target) < EPS_ZERO) return target;
		return res;
	}

	template<class Function>
	requires std::is_same_v<bool, std::invoke_result_t<Function, float>> constexpr float
		binsearch(float left, float right, const Function& function, const float EPS = EPS_ZERO) {
		float mid;
		while (math::isZero(left - right, EPS)) {
			mid = (left + right) / 2.f;
			if (function)
				left = mid;
			else
				right = mid;
		}
		return left;
	}
}  // namespace mk::Math
