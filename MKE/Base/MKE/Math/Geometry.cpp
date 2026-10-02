//
// Created by mateusz on 9/16/23.
//

#include "Geometry.hpp"

#include "Base.hpp"
#include "Vector.hpp"

#include <iostream>
#include <limits>
#include <vector>

namespace mk::math {

	float dotProduct(const Vector2f& vec1, const Vector2f& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	namespace {

		// here vectors are points
		float determinant(const Vector2f& tail, const Vector2f& head1, const Vector2f& head2) {
			return (head1.x - tail.x) * (head2.y - tail.y)
			     - (head2.x - tail.x) * (head1.y - tail.y);
		}

		struct Projection {
			float min, max;
		};

		Projection project(const std::vector<Vector2f>& shape, Vector2f axis) {
			float min = dotProduct(shape[0], axis);
			float max = min;
			for (usize i = 1; i < shape.size(); i++) {
				const float d = dotProduct(shape[i], axis);
				min           = std::min(min, d);
				max           = std::max(max, d);
			}
			return { min, max };
		}
	}

	Vector2f getPerpendicular(const Vector2f& vec) { return { -vec.y, vec.x }; }

	bool isPointInsideConvex(const std::vector<Vector2f>& convex, const Vector2f& point) {
		if (convex.size() < 3) return false;

		bool has_neg = false;
		bool has_pos = false;
		for (std::size_t i = 0; i < convex.size(); i++) {
			int now_sign = sign(determinant(point, convex[i], convex[(i + 1) % convex.size()]));
			if (now_sign == -1) has_neg = true;
			if (now_sign == 1) has_pos = true;
			if (has_neg && has_pos) return false;
		}
		return true;
	}

	bool isPointInsidePolygon(const std::vector<Vector2f>& poly, const Vector2f& p) {
		bool inside = false;
		for (std::size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
			const Vector2f& a = poly[i];
			const Vector2f& b = poly[j];
			// Does edge (a, b) straddle the horizontal line through p,
			// and is the crossing point to the right of p?
			if ((a.y > p.y) != (b.y > p.y) && p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x)
				inside = !inside;
		}
		return inside;
	}

	std::optional<CollisionInfo>
		getCollision(const std::vector<Vector2f>& shape1, const std::vector<Vector2f>& shape2) {
		float    depth = std::numeric_limits<float>::infinity();
		Vector2f normal;

		for (const auto& shape: { &shape1, &shape2 })
			for (usize i = 0; i < shape->size(); i++) {
				const Vector2f& start = (*shape)[i];
				const Vector2f& end   = (*shape)[(i + 1) % shape->size()];
				Vector2f        axis  = getPerpendicular(end - start);
				if (axis.lengthSquared() < EPS_ZERO) continue;
				axis = axis.normalizeOrZero();

				const Projection pa = project(shape1, axis);
				const Projection pb = project(shape2, axis);
				if (pa.max <= pb.min || pb.max <= pa.min) return std::nullopt;

				const float push_a  = pa.max - pb.min;
				const float push_b  = pb.max - pa.min;
				const float overlap = std::min(push_a, push_b);
				if (overlap < depth) {
					depth  = overlap;
					normal = (push_a < push_b) ? -axis : axis;
				}
			}
		return { { normal, depth } };
	}

	Vector2f findLineIntersection(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4) {
		float px = ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x)
		            - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x))
		         / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));

		float py = ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y)
		            - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x))
		         / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));

		return { px, py };
	}

	// Bresenham algorithm
	std::vector<Vector2i> drawLine(Vector2i start, Vector2i end) {
		if (start == end) return { start };

		Vector2i currentPosition = start;
		Vector2f dirVec          = normalizeVector((end - start).type<float>());
		Vector2f step            = { dirVec.y / dirVec.x, dirVec.x / dirVec.y };
		Vector2f stepLength  = { std::sqrt(step.x * step.x + 1), std::sqrt(step.y * step.y + 1) };
		Vector2f rayProgress = { 0, 0 };

		float distance    = 0;
		float maxDistance = (end - start).length();

		std::vector<Vector2i> points;

		while (distance < maxDistance) {
			points.push_back(currentPosition);
			if (rayProgress.x < rayProgress.y) {
				currentPosition.x += sign(dirVec.x);
				distance = rayProgress.x;
				rayProgress.x += stepLength.x;
			} else {
				currentPosition.y += sign(dirVec.y);
				distance = rayProgress.y;
				rayProgress.y += stepLength.y;
			}
		}

		return points;
	}

}  // namespace mk::Math
