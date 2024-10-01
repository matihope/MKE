#pragma once

#include "MKE/Math/Vector2.hpp"
#include "MKE/Color.hpp"

namespace mk {
	struct Vertex {
		virtual ~Vertex() = 0;
		virtual void configureVertexAttribute() = 0;
	};
	inline Vertex::~Vertex() {}

	struct Vertex2D: public Vertex {
		math::Vector2f position; // 8 bytes
		Color          color; // 4 bytes
		math::Vector2f tex_coords; // 4 bytes

		void configureVertexAttribute() override;
	};

}
