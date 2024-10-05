#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class RectPrimitive: public Transformable, public Drawable2D, public NonCopyable {
	public:
		RectPrimitive(math::Vector2f size);
		RectPrimitive(float width, float height);

		void draw2d(const RenderTarget& target, DrawContext2D context) const override;

	private:
		VertexArray2D vertices{ true };
        math::Vector2f size;
	};
}
