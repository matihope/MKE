//
// Created by mateusz on 8/30/23.
//

#pragma once

#include "MKE/Color.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {

	class RectShape: public WorldEntity {
	public:
		RectShape() = default;
		RectShape(Color myColor, math::Vector2f size);
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setColor(const sf::Color& color);

		void setSize(const mk::math::Vector2f& size);

		mk::math::Vector2f getSize() const;

	private:
		sf::RectangleShape rect;
	};

}  // namespace mk
