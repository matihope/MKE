#pragma once

#include "MKE/Collisions/CollisionShape/CollisionShape.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include "MKE/Nodes/GUI/Clickable.hpp"
#include "MKE/Nodes/GUI/Label.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk::gui {
	class Button: public WorldEntityUI, public Clickable {
		Button();

		Label     m_label;
		RectShape m_background;

		Color m_background_color_normal = Color(60, 60, 60);
		Color m_background_color_hover  = Color(50, 50, 50);
		Color m_background_color_press  = Color(40, 40, 40);

		Color m_font_color_normal = Color(255, 255, 255);
		Color m_font_color_hover  = Color(200, 200, 200);
		Color m_font_color_press  = Color(125, 125, 125);

		std::unique_ptr<RectCollision> m_collision_shape;
		bool                           m_has_custom_collision_shape = false;

		HAlignment m_halignment = HAlignment::LEFT;
		VAlignment m_valignment = VAlignment::TOP;

		// minimal size of the button background
		math::Vector2f minSize = { -1, -1 };

		// space between font border end and an edge
		math::Vector2f minSpaceBetween = { -1, -1 };

		void fixLabelPosition();

	protected:
		void onStopHover() override;
		void onHover() override;
		void onHold() override;

	public:
		Button(Font* font, const std::string& text);

		void setAlignment(HAlignment newHAlignment, VAlignment newVAlignment);

		void setPositionBase(math::Vector3f position) override;

		void setFont(Font* font);
		void setText(const std::string& newText);
		void setTextSize(unsigned int newSize);

		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

		void onUpdate(Game& game, float dt) override { Clickable::update(game, dt); }

		void        updateDefaultCollisionShape();
		void        setCollisionShape(std::unique_ptr<RectCollision> shape);
		math::RectF getBounds() const;

		void setBackgroundColors(Color normal, Color hover, Color press);
		void setBackgroundColors(Color colors);
		void setFontColors(Color normal, Color hover, Color press);
		void setFontColors(Color colors);

		void setMinSpaceBetween(math::Vector2f space);
		void setMinSize(math::Vector2f size);
	};
}  // namespace mk::GUI
