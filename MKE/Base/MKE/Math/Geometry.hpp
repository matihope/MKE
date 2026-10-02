#pragma once

#include "Vector.hpp"

#include <vector>

namespace mk::math {
	bool doLinesIntersect(
		Vector2f lineAStart, Vector2f lineAEnd, Vector2f LineBStart, Vector2f LineBEnd
	);
	Vector2f findLineIntersection(
		Vector2f lineAStart, Vector2f lineAEnd, Vector2f LineBStart, Vector2f LineBEnd
	);

	float dotProduct(const Vector2f& vec1, const Vector2f& vec2);

	std::vector<Vector2i> drawLine(Vector2i start, Vector2i end);

	Vector2f getPerpendicular(const Vector2f& vector);

	bool isPointInsideConvex(const std::vector<Vector2f>& convex, const Vector2f& point);
	bool isPointInsidePolygon(const std::vector<Vector2f>& poly, const Vector2f& p);

	struct CollisionInfo {
		Vector2f normal;
		float    depth;
	};

	std::optional<CollisionInfo>
		getCollision(const std::vector<Vector2f>& shape1, const std::vector<Vector2f>& shape2);
}
