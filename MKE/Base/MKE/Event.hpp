#pragma once

#include "MKE/Input.hpp"
#include "Math/Vector.hpp"
#include <variant>

namespace mk {
	class Event {
	public:
		struct WindowClose {};

		struct WindowResized {
			math::Vector2u new_size;
		};

		struct WindowScaleFactorChanged {
			math::Vector2f scale_factors;
		};

		struct KeyPressed {
			input::KEY key;
		};

		struct KeyReleased {
			input::KEY key;
		};

		struct MouseButtonPressed {
			input::MOUSE button;
		};

		struct MouseButtonReleased {
			input::MOUSE button;
		};

		struct MouseMoved {
			math::Vector2f new_position;
		};

		struct MouseMotionDelta {
			math::Vector2f delta;
		};

		struct MouseScrolled {
			math::Vector2f delta;
		};

		template<class Ev>
		Event(const Ev& e): data(e) {}

		Event() = default;

		template<class Ev>
		[[nodiscard]]
		std::optional<Ev> get() const {
			if (std::holds_alternative<Ev>(data)) return std::optional<Ev>(std::get<Ev>(data));
			return {};
		}

		template<class Ev>
		[[nodiscard]]
		bool is() const {
			return std::holds_alternative<Ev>(data);
		}

	private:
		std::variant<
			WindowClose,
			WindowResized,
			WindowScaleFactorChanged,
			KeyPressed,
			KeyReleased,
			MouseButtonPressed,
			MouseButtonReleased,
			MouseMoved,
			MouseMotionDelta,
			MouseScrolled>
			data{};
	};
}
