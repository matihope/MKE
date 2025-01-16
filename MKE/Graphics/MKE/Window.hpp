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

		[[nodiscard]]
		virtual math::Vector2f getScalingFactor() const;

		[[nodiscard]]
		virtual math::Vector2u getSize() const;

		void clear(Color color);
		void display();

		virtual void addEvent(Event event);
		bool         pollEvent(Event& event);

		[[nodiscard]]
		bool isKeyPressed(input::KEY key) const;
		[[nodiscard]]
		bool isMousePressed(input::MOUSE key) const;
		[[nodiscard]]
		math::Vector2i getMousePosition() const;

		void enableVerticalSync(bool enable);

		[[nodiscard]]
		bool isExitRequested() const;

		void setExitRequested(bool value);

		enum class MouseMode {
			NORMAL,
			HIDDEN,
			GRABBED  // (GLFW Disabled)
		};
		void setMouseCursorMode(MouseMode mode);
		[[nodiscard]]
		MouseMode getMouseCursorMode() const;


	protected:
		bool              exit_requested = false;
		bool              initialized    = false;
		std::string       title{};
		math::Vector2u    window_size{};
		math::Vector2f    window_scale_factor{};
		GLFWwindow*       window = nullptr;
		std::queue<Event> events;

		math::Vector2f mouse_position{ -1.f };

		MouseMode mouse_mode = MouseMode::NORMAL;

		// This should not be public, if you wish to you use it please use a method from the
		// interface, and if there's none, then add it please.
		[[nodiscard]]
		GLFWwindow* getNativeHandle() const;
	};

}
