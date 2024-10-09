#pragma once

#include "MKE/Color.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class CubePrimitive: public Transformable, public Drawable3D {
	public:
		CubePrimitive(mk::Color color = Colors::WHITE);
		void draw3d(const RenderTarget3D& target, DrawContext context) const override;

	private:
		VertexArray3D vertex_array{ false };
	};

}
