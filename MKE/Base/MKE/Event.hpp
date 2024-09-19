#pragma once

#include "Math/Vector2.hpp"

namespace mk {
	enum class EventType { WindowClose, KeyPressed, WindowResized };

	namespace Events {
		struct WindowClose {
			WindowClose(): type(EventType::WindowClose) {};
			EventType type;
		};

		struct KeyPressed {
			KeyPressed(): type(EventType::KeyPressed) {};
			EventType type;
			// code
		};

		struct WindowResized {
			WindowResized(math::Vector2u new_size):
				  type(EventType::WindowResized),
				  new_size(new_size) {};
			EventType      type;
			math::Vector2u new_size;
		};
	}

	union Event {
        Event() {}
		EventType             type;  // As everywhere
		Events::WindowClose   window_close;
		Events::KeyPressed    key_pressed;
		Events::WindowResized window_resized;
	};
}
