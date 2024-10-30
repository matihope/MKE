#pragma once

#include "Color.hpp"
#include "MKE/Event.hpp"
#include "MKE/NonCopyable.hpp"
#include <MKE/Math/Vector.hpp>
#include <queue>

struct GLFWwindow;

namespace mk {
	class Window: public NonCopyable {
	public:
		Window() = default;
		Window(u32 width, u32 height, std::string_view title);
		Window(math::Vector2u size, std::string_view title);

		virtual ~Window();

		void create(u32 width, u32 height, std::string_view title);
		void create(math::Vector2u size, std::string_view title);

		void setSize(math::Vector2u size);
		void setSize(unsigned int width, unsigned int height);

		math::Vector2f getScalingFactor() const;

		math::Vector2u getSize() const;

		void clear(Color color);
		void display();

		virtual void addEvent(Event event);
		bool         pollEvent(Event& event);

		bool isKeyPressed(input::KEY key) const;
		bool isKeyJustPressed(input::KEY key) const;
		bool isMousePressed(input::MOUSE key) const;
		bool isMouseJustPressed(input::MOUSE key) const;
		math::Vector2i getMousePosition() const;

		void enableVerticalSync(bool enable);

		bool isExitRequested() const;

		void setExitRequested(bool value);

	protected:
		bool              exit_requested = false;
		bool              initialized    = false;
		std::string       title{};
		math::Vector2u    window_size{};
		math::Vector2f    window_scale_factor{};
		GLFWwindow*       window = nullptr;
		std::queue<Event> events;

		// A map of {key, just_pressed}
		std::array<bool, ((usize) input::KEY::_LAST - (usize) input::KEY::_FIRST) + 1>
			just_pressed_key{};

		std::array<bool, ((usize) input::MOUSE::BUTTON_8 - (usize) input::MOUSE::BUTTON_1) + 1>
			just_pressed_mouse{};
	};
}
