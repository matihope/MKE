#pragma once

#include "MKE/Math/Matrix.hpp"
#include <cmath>

namespace mk::math {

	constexpr Matrix4f perspective(float fovX, float aspect_ratio, float near, float far) {
		Matrix4f projection{ 1.f };
		float    DEG2RAD = 2.f * M_PI / 360.f;

		float tangent = std::tan(fovX / 2 * DEG2RAD);
		float right   = near * tangent;
		float top     = right / aspect_ratio;

		projection(0, 0) = near / right;
		projection(1, 1) = near / top;
		projection(2, 2) = (near + far) / (near - far);
		projection(3, 2) = -1.f;
		projection(2, 3) = 2 * far * near / (near - far);
		projection(3, 3) = 0.f;

		// float scale = 1 / std::tan(fovX * 0.5 * DEG2RAD);
		// projection(0, 0) = scale;  // scale the x coordinates of the projected point
		// projection(1, 1) = scale;  // scale the y coordinates of the projected point
		// projection(2, 2) = -far / (far - near);  // used to remap z to [0,1]
		// projection(2, 3) = -far * near / (far - near);  // used to remap z [0,1]
		// projection(3, 2) = -1;  // set w = -z
		// projection(3, 3) = 0;

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

	constexpr Matrix4f camera2D(float left, float right, float top, float bottom) {
		Matrix4f projection{ 1.f };
		projection(0, 0) = 2.f / (right - left);
		projection(1, 1) = 2.f / (top - bottom);
		projection(3, 0) = -(right + left) / (right - left);
		projection(3, 1) = -(top + bottom) / (top - bottom);

		return projection;
	}

	constexpr Matrix4f orthogonal([[maybe_unused]] float width, [[maybe_unused]] float height) {
		Matrix4f projection{ 1.f };

		projection(0, 0) = 2.f / width * height;
		projection(1, 1) = 2.f;

		return projection;
	}

	constexpr Matrix4f lookAtDirection(Vector3f from, Vector3f direction, Vector3f up) {
		direction = -direction.normalizeOrZero();

		Vector3f right_vec = cross(up, direction).normalizeOrZero();
		Vector3f up_vec    = cross(direction, right_vec).normalizeOrZero();

		Matrix4f look_at{ 1.f };
		look_at(0, 0) = right_vec.x;
		look_at(0, 1) = right_vec.y;
		look_at(0, 2) = right_vec.z;

		look_at(1, 0) = up_vec.x;
		look_at(1, 1) = up_vec.y;
		look_at(1, 2) = up_vec.z;

		look_at(2, 0) = direction.x;
		look_at(2, 1) = direction.y;
		look_at(2, 2) = direction.z;

		Matrix4f translation{ 1.f };
		translation(0, 3) = -from.x;
		translation(1, 3) = -from.y;
		translation(2, 3) = -from.z;

		return look_at * translation;
	}

	constexpr Matrix4f lookAt(Vector3f from, Vector3f at, Vector3f up) {
		return lookAtDirection(from, at - from, up);
	}
}
