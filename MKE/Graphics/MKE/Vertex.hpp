#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/Color.hpp"

namespace mk {
	struct Vertex2D {
		math::Vector2f position;    // 8 bytes
		Color          color;       // 16 bytes
		math::Vector2f tex_coords;  // 8 bytes

		static void configureVertexAttribute();
	};

	struct Vertex3D {
		math::Vector3f position;    // 12 bytes
		Color          color;       // 16 bytes
		math::Vector2f tex_coords;  // 8 bytes

		static void configureVertexAttribute();
	};
}
