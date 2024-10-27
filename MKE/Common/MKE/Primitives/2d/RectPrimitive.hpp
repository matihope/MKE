#pragma once

#include "MKE/Transformable.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class RectPrimitive: public Transformable, public Drawable {
	public:
		RectPrimitive(math::Vector2f size);
		RectPrimitive(float width, float height);

		void draw(const RenderTarget& target, DrawContext context) const override;

	private:
		VertexArray2D  vertices{ true };
		math::Vector2f size;
	};
}
