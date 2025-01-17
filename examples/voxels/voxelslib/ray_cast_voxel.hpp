#pragma once
/*
 * Implementation of voxel traversal algorithm from "A Fast Voxel Traversal
 * Algorithm for Ray Tracing" (Amanatides and Woo).
 */


#include "MKE/Math/Vector.hpp"


#include <limits>
#include <algorithm>
#include <cmath>
#include <vector>

namespace voxels {
	using Vector3d = mk::math::Vector3<double>;

	constexpr std::vector<mk::math::Vector3i>
		castVoxelRay(const Vector3d& origin, const Vector3d& direction, const double max_len) {
		auto [x, y, z]
			= Vector3d(std::floor(origin.x), std::floor(origin.y), std::floor(origin.z)).vec_data;

		const mk::math::Vector3i step = { mk::math::sign(direction.x),
			                              mk::math::sign(direction.y),
			                              mk::math::sign(direction.z) };

		const Vector3d cellBoundary
			= { x + (step.x > 0 ? 1 : 0), y + (step.y > 0 ? 1 : 0), z + (step.z > 0 ? 1 : 0) };

		Vector3d tMax = (cellBoundary - origin) / direction;
		if (tMax.x == NAN) tMax.x = INFINITY;
		if (tMax.y == NAN) tMax.y = INFINITY;
		if (tMax.z == NAN) tMax.z = INFINITY;

		Vector3d tDelta = step.type<double>() / direction;
		if (tDelta.x == NAN) tDelta.x = INFINITY;
		if (tDelta.y == NAN) tDelta.y = INFINITY;
		if (tDelta.z == NAN) tDelta.z = INFINITY;

		std::vector<mk::math::Vector3i> result;
		while (true) {
			result.emplace_back(x, y, z);
			if ((result.back().type<double>() - origin).lengthSquared() > max_len * max_len) {
				result.pop_back();
				return result;
			}
			if (tMax.x < tMax.y && tMax.x < tMax.z) {
				// tMax.X is the lowest, an YZ cell boundary plane is nearest.
				x += step.x;
				tMax.x += tDelta.x;
			} else if (tMax.y < tMax.z) {
				// tMax.Y is the lowest, an XZ cell boundary plane is nearest.
				y += step.y;
				tMax.y += tDelta.y;
			} else {
				// tMax.Z is the lowest, an XY cell boundary plane is nearest.
				z += step.z;
				tMax.z += tDelta.z;
			}
		}
	}
}
