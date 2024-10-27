//
// Created by mateusz on 8/31/23.
//

#pragma once

#include "MKE/WorldEntity.hpp"

namespace mk {
	class RayShape: public WorldEntity {
	public:
		RayShape() = default;
		RayShape(Color color, float width, float length, float wingLength);
		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;
		void setSize(float width, float length, float wingLength);
		void setColor(Color color);
		void pointAt(math::Vector2f arrowHead);

	private:
		float              width{}, length{}, wingLength{};
		sf::RectangleShape body;
		sf::RectangleShape leftWing, rightWing;
		sf::CircleShape    arrowFill;
	};
}  // namespace mk
