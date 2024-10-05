#pragma once

#include "MKE/Input.hpp"
#include "Math/Vector.hpp"

namespace mk {
	enum class EventType {
		InvalidEvent = 0,
		WindowClose,
		WindowResized,
		WindowScaleFactorChanged,
		KeyPressed,
		KeyReleased,
	};

	namespace Events {
		struct WindowClose {
			WindowClose(): type(EventType::WindowClose) {};
			EventType type;
		};

		struct WindowResized {
			WindowResized(math::Vector2u new_size):
				  type(EventType::WindowResized),
				  new_size(new_size) {}

			EventType      type;
			math::Vector2u new_size;
		};

		struct WindowScaleFactorChanged {
			WindowScaleFactorChanged(math::Vector2f new_scale):
				  type(EventType::WindowScaleFactorChanged),
				  scale_factors(new_scale) {}

			EventType      type;
			math::Vector2f scale_factors;
		};

		struct KeyPressed {
			KeyPressed(input::KEY key): type(EventType::KeyPressed), key(key) {};
			EventType  type;
			input::KEY key;
		};

		struct KeyReleased {
			KeyReleased(input::KEY key): type(EventType::KeyReleased), key(key) {};
			EventType  type;
			input::KEY key;
		};
	}

	union Event {
		Event() {};

		EventType                        type{};  // As everywhere
		Events::WindowClose              window_close;
		Events::WindowResized            window_resized;
		Events::WindowScaleFactorChanged window_scale_factor;
		Events::KeyPressed               key_pressed;
		Events::KeyReleased              key_released;
	};
}
