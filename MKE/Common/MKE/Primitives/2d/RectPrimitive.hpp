#pragma once

#include "MKE/Color.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class RectPrimitive: public Transformable, public Drawable {
	public:
		RectPrimitive(math::Vector2f size);
		RectPrimitive(float width, float height);

		~RectPrimitive() = default;

		void setSize(float width, float height);
		void setSize(math::Vector2f size);

		math::Vector2f getSize() const;

		void setColor(Color color);

	private:
		void draw(RenderTarget& target, DrawContext context) const override;

		void updateVertices();

		Color          color = Colors::WHITE;
		VertexArray2D  vertices{ true };
		math::Vector2f size;
	};
}
