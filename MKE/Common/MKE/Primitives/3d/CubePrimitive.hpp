#pragma once

#include "MKE/Color.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class CubePrimitive: public Transformable, public Drawable {
	public:
		CubePrimitive(Color color = Colors::WHITE);

		void setColor(Color color);

	private:
		void draw(RenderTarget& target, DrawContext context) const override;

		void           updateVertices();
		VertexArray3D  vertex_array{ false };
		Color          color;
	};

}
