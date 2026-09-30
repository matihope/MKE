#pragma once

#include "MKE/Color.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class CirclePrimitive: public Transformable, public Drawable {
	public:
		CirclePrimitive(float radius, usize point_count = 30);
		~CirclePrimitive() = default;

		void setRadius(float radius);
		void setPointCount(usize point_count);

		float getRadius() const;
		usize getPointCount() const;

		void  setColor(Color color);
		Color getColor(Color color);

	private:
		void draw(RenderTarget& target, DrawContext context) const override;

		void updateVertices();

		Color color = Colors::WHITE;
		float radius;
		usize point_count;

		VertexArray2D vertices{ true };
	};
}
