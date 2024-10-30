#include "Window.hpp"
#include "MKE/Init.hpp"
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Panic.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>

#include <unordered_map>

namespace {
	std::unordered_map<GLFWwindow*, std::reference_wrapper<mk::Window>> glfw_to_mk_window{};

	void pushEvent(GLFWwindow* window, mk::Event event) {
		glfw_to_mk_window.at(window).get().addEvent(event);
	}

	void windowCloseCallback(GLFWwindow* window) {
		mk::Event event{};
		event.window_close = mk::Events::WindowClose();
		pushEvent(window, event);
	}

	void windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		mk::Event event{};
		event.window_resized = mk::Events::WindowResized(mk::math::Vector2u(width, height));
		pushEvent(window, event);
	}

	void windowScaleFactorCallback(GLFWwindow* window, float xscale, float yscale) {
		mk::Event event{};
		event.window_scale_factor = mk::Events::WindowScaleFactorChanged({ xscale, yscale });
		pushEvent(window, event);
	}

	void windowKeyCallback(
		GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */
	) {
		mk::Event event;

		// This is fine, because mk::input::KEY follows GLFW's key ordering.
		if (action == GLFW_PRESS)
			event.key_pressed = mk::Events::KeyPressed(mk::input::KEY(key));
		else if (action == GLFW_RELEASE)
			event.key_released = mk::Events::KeyReleased(mk::input::KEY(key));
		else
			return;

		pushEvent(window, event);
	}

	void windowMouseCallback(
		GLFWwindow* window, int button, int action, int /* mods */
	) {
		mk::Event event;

		// This is fine, because mk::input::BUTTON follows GLFW's key ordering.
		if (action == GLFW_PRESS)
			event.mouse_pressed = mk::Events::MouseButtonPressed(mk::input::MOUSE(button));
		else if (action == GLFW_RELEASE)
			event.mouse_released = mk::Events::MouseButtonReleased(mk::input::MOUSE(button));
		else
			return;

		pushEvent(window, event);
	}

	[[maybe_unused]]
	mk::math::Vector2u getGlViewportSize() {
		i32 data[4];
		glGetIntegerv(GL_VIEWPORT, &data[0]);
		return { u32(data[2]), u32(data[3]) };
	}
}

mk::Window::Window(u32 width, u32 height, std::string_view title) { create(width, height, title); }

mk::Window::Window(math::Vector2u size, std::string_view title) { create(size, title); }

void mk::Window::create(u32 width, u32 height, std::string_view title) {
	MK_ASSERT_TRUE(!initialized, "Called create on initalized window");

	window = mk::init::getWindow();
	glfw_to_mk_window.insert({ window, (*this) });

	this->title = title;

	glfwSetWindowSize(window, width, height);
	glfwSetWindowTitle(window, title.begin());

	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
	glfwSetWindowContentScaleCallback(window, windowScaleFactorCallback);
	glfwSetKeyCallback(window, windowKeyCallback);
	glfwSetMouseButtonCallback(window, windowMouseCallback);

	glfwGetWindowContentScale(window, &window_scale_factor.x, &window_scale_factor.y);
	MK_ASSERT_TRUE(window_scale_factor.x > 0, "Invalid window native x_scale");
	MK_ASSERT_TRUE(window_scale_factor.y > 0, "Invalid window native y_scale");

	glEnable(GL_DEPTH_TEST);

	std::cerr << " -- Requested window size: " << math::Vector2u{ width, height } << '\n';
	std::cerr << " -- Guessed window scaling factor: " << window_scale_factor << '\n';

	initialized = true;
	setSize(width, height);
}

void mk::Window::create(math::Vector2u size, std::string_view title) {
	create(size.x, size.y, title);
}

void mk::Window::setSize(math::Vector2u size) {
	window_size = size;

	math::Vector2f new_size = size.type<float>() * window_scale_factor;
	if (initialized) glViewport(0, 0, new_size.x, new_size.y);
}

void mk::Window::setSize(u32 width, u32 height) { setSize({ width, height }); }

void mk::Window::display() {
	glfwSwapBuffers(window);
	just_pressed_key.fill(false);
	just_pressed_mouse.fill(false);
	glfwPollEvents();
}

void mk::Window::addEvent(Event event) {
	if (event.type == EventType::WindowScaleFactorChanged)
		window_scale_factor = event.window_scale_factor.scale_factors;

	else if (event.type == EventType::WindowResized) {
		event.window_resized.new_size
			= (event.window_resized.new_size.type<float>() / window_scale_factor).type<usize>();
		setSize(event.window_resized.new_size);
	} else if (event.type == EventType::WindowClose) {
		setExitRequested(true);
	}

	else if (event.type == EventType::KeyPressed) {
		just_pressed_key[(usize) event.key_pressed.key] = true;
	} else if (event.type == EventType::MouseButtonPressed) {
		just_pressed_mouse[(usize) event.mouse_pressed.button] = true;
	}

	events.push(event);
}

bool mk::Window::pollEvent(Event& event) {
	if (!events.empty()) {
		event = events.front();
		events.pop();
		return true;
	}
	return false;
}

void mk::Window::clear(Color color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

mk::math::Vector2u mk::Window::getSize() const { return window_size; }

void mk::Window::enableVerticalSync(bool enable) {
	if (enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

mk::math::Vector2f mk::Window::getScalingFactor() const { return window_scale_factor; }

bool mk::Window::isExitRequested() const { return exit_requested; }

void mk::Window::setExitRequested(bool value) { exit_requested = value; }

bool mk::Window::isKeyPressed(input::KEY key) const {
	return glfwGetKey(window, (i32) key) == GLFW_PRESS;
}

bool mk::Window::isKeyJustPressed(input::KEY key) const { return just_pressed_key[(usize) key]; }

mk::Window::~Window() { glfwDestroyWindow(window); }

bool mk::Window::isMousePressed(input::MOUSE key) const {
	return glfwGetMouseButton(window, (i32) key) == GLFW_PRESS;
}

bool mk::Window::isMouseJustPressed(input::MOUSE key) const {
	return just_pressed_mouse[(usize) key];
}

mk::math::Vector2i mk::Window::getMousePosition() const {
	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	return math::Vector2i(xpos, ypos);
}
