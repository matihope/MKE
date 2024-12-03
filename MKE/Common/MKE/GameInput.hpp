#pragma once
#include "MKE/Event.hpp"

namespace mk {
	class GameInput {
	public:
		void handleEvent(const Event& event);

		bool isKeyPressed(input::KEY key) const;
		bool isKeyJustPressed(input::KEY key) const;
		bool isKeyJustReleased(input::KEY key) const;
		bool isMousePressed(input::MOUSE key) const;
		bool isMouseJustPressed(input::MOUSE key) const;
		bool isMouseJustReleased(input::MOUSE key) const;

		void tick();

	private:
		// A map of {key, just_pressed}
		std::array<bool, ((usize) input::KEY::_LAST - (usize) input::KEY::_FIRST) + 1>
			pressed_key{};

		std::array<bool, ((usize) input::MOUSE::BUTTON_8 - (usize) input::MOUSE::BUTTON_1) + 1>
			pressed_mouse{};

		std::array<bool, ((usize) input::KEY::_LAST - (usize) input::KEY::_FIRST) + 1>
			prev_pressed_key{};

		std::array<bool, ((usize) input::MOUSE::BUTTON_8 - (usize) input::MOUSE::BUTTON_1) + 1>
			prev_pressed_mouse{};
	};
}
