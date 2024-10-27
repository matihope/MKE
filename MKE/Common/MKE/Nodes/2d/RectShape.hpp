//
// Created by mateusz on 8/30/23.
//

#pragma once

#include "MKE/Color.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {

	class RectShape: public WorldEntity2D {
	public:
		RectShape();
		~RectShape() = default;
		RectShape(Color color, math::Vector2f size);

		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

		void setColor(Color color);

		void setSize(const mk::math::Vector2f& size);

		mk::math::Vector2f getSize() const;

	private:
		mk::RectPrimitive rect;
	};

}  // namespace mk
