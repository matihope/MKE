#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class RectPrimitive: public Transformable, public Drawable2D {
	public:
		RectPrimitive();

		void draw2d(const RenderTarget& target, DrawContext2D context) const override;

	private:
		VertexArray2D vertices{ true };
	};
}
