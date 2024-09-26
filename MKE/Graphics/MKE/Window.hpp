#pragma once

#include "Color.hpp"
#include "MKE/Event.hpp"
#include "MKE/NonCopyable.hpp"
#include <MKE/Math/Vector2.hpp>

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

		math::Vector2u getSize() const;

		void clear(Color color);
		void display();

		void addEvent(Event event);
		bool pollEvent(Event& event);

		bool isKeyPressed(input::KEY key) const;

		void enableVerticalSync(bool enable);

	private:
		bool              initialized = false;
		std::string       title{};
		math::Vector2u    window_size{};
		GLFWwindow*       window = nullptr;
		std::queue<Event> events;
	};
}
