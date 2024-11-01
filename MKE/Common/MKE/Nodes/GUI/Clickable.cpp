#include "Clickable.hpp"
#include "MKE/Game.hpp"
#include "MKE/Input.hpp"

namespace mk {
	Clickable::~Clickable() = default;

	void Clickable::update(Game& game, float) {
		if (!collision_shape) return;
		math::Vector2f mousePos = game.getMousePos();
		is_help                 = false;
		is_pressed              = false;

		if (collision_shape->contains(mousePos)) {
			if (game.isMousePressed(input::MOUSE_LEFT)) {
				if (is_pressable) {
					is_help = true;
					onHold();
					if (not was_help_prev and click_mode == ClickMode::PressOnClick) makePress();
				}
			} else {
				is_pressable = true;
				if (was_help_prev) {
					if (click_mode == ClickMode::PressOnRelease) makePress();
					onRelease();
				} else {
					onHover();
				}
			}
		} else {
			is_pressable = false;
			onStopHover();
		}
		was_help_prev = is_help;
	}

	void Clickable::setClickCollisionShape(CollisionComponent* collision_shape) {
		this->collision_shape = collision_shape;
	}

	void Clickable::setClickMode(Clickable::ClickMode click_mode) { this->click_mode = click_mode; }

	bool Clickable::isPressed() const { return is_pressed; }

	void Clickable::makePress() {
		is_pressed = true;
		onPressed();
	}
}  // namespace mk
