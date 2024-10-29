#pragma once

#include "MKE/Math/Matrix.hpp"
#include <cmath>

namespace mk::math {

	constexpr Matrix4f perspective(float fovX, float aspect_ratio, float front, float back) {
		Matrix4f projection{ 1.f };
		float    DEG2RAD = 2.f * M_PI / 360.f;

		float tangent = std::tan(fovX / 2 * DEG2RAD);
		float right   = front * tangent;
		float top     = right / aspect_ratio;

		projection(0, 0) = front / right;
		projection(1, 1) = front / top;
		projection(2, 2) = (front + back) / (front - back);
		projection(3, 2) = -1.f;
		projection(2, 3) = 2 * back * front / (front - back);
		projection(3, 3) = 0.f;

		return projection;
	}

	constexpr Matrix4f camera2D([[maybe_unused]] float width, [[maybe_unused]] float height) {
		Matrix4f projection{ 1.f };

		projection(0, 0) = 2.f / width;
		projection(1, 1) = -2.f / height;
		projection(0, 3) = -1;
		projection(1, 3) = 1;

		return projection;
	}

	constexpr Matrix4f orthogonal([[maybe_unused]] float width, [[maybe_unused]] float height) {
		Matrix4f projection{ 1.f };

		projection(0, 0) = 2.f / width * height;
		projection(1, 1) = 2.f;

		return projection;
	}

	constexpr Matrix4f lookAt(Vector3f from, Vector3f at, Vector3f up) {
		Vector3f forward_vec = (from - at).normalizeOrZero();
		Vector3f right_vec   = cross(up, forward_vec).normalizeOrZero();
		Vector3f up_vec      = cross(forward_vec, right_vec).normalizeOrZero();

		Matrix4f look_at{ 1.f };
		look_at(0, 0) = right_vec.x;
		look_at(0, 1) = right_vec.y;
		look_at(0, 2) = right_vec.z;

		look_at(1, 0) = up_vec.x;
		look_at(1, 1) = up_vec.y;
		look_at(1, 2) = up_vec.z;

		look_at(2, 0) = forward_vec.x;
		look_at(2, 1) = forward_vec.y;
		look_at(2, 2) = forward_vec.z;

		Matrix4f translation{ 1.f };
		translation(0, 3) = -from.x;
		translation(1, 3) = -from.y;
		translation(2, 3) = -from.z;

		return look_at * translation;
	}
}
