#include "Window.hpp"
#include "MKE/Init.hpp"
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Panic.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>

#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_map>

namespace {
	std::unordered_map<GLFWwindow*, mk::Window*> glfw_to_mk_window{};

	void pushEvent(GLFWwindow* window, mk::Event event) {
		glfw_to_mk_window[window]->addEvent(event);
	}

	void windowCloseCallback(GLFWwindow* window) { pushEvent(window, mk::Event::WindowClose{}); }

	void windowPosCallback(GLFWwindow* window, int /* xpos */, int /* ypos */) {
		glfw_to_mk_window[window]->checkMonitorChange();
	}

	void windowFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		pushEvent(window, mk::Event::WindowResized(mk::math::Vector2u(width, height)));
	}

	void windowScaleFactorCallback(GLFWwindow* window, float xscale, float yscale) {
		pushEvent(window, mk::Event::WindowScaleFactorChanged({ xscale, yscale }));
	}

	void windowKeyCallback(
		GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */
	) {
		mk::Event event;

		// This is fine, because mk::input::KEY follows GLFW's key ordering.
		if (action == GLFW_PRESS)
			event = mk::Event::KeyPressed(static_cast<mk::input::KEY>(key));
		else if (action == GLFW_RELEASE)
			event = mk::Event::KeyReleased(static_cast<mk::input::KEY>(key));
		else
			return;

		pushEvent(window, event);
	}

	void windowMouseButtonCallback(
		GLFWwindow* window, int button, int action, int /* mods */
	) {
		mk::Event event;

		// This is fine, because mk::input::BUTTON follows GLFW's key ordering.
		if (action == GLFW_PRESS)
			event = mk::Event::MouseButtonPressed(static_cast<mk::input::MOUSE>(button));
		else if (action == GLFW_RELEASE)
			event = mk::Event::MouseButtonReleased(static_cast<mk::input::MOUSE>(button));
		else
			return;

		pushEvent(window, event);
	}

	void windowCursorPosCallback(GLFWwindow* window, double x_pos, double y_pos) {
		pushEvent(
			window, mk::Event::MouseMoved({ static_cast<float>(x_pos), static_cast<float>(y_pos) })
		);
	}

	void windowScrollCallback(GLFWwindow* window, const double xoffset, const double yoffset) {
		pushEvent(
			window,
			mk::Event::MouseScrolled({ static_cast<float>(xoffset), static_cast<float>(yoffset) })
		);
	}

	[[maybe_unused]]
	mk::math::Vector2u getGlViewportSize() {
		i32 data[4];
		glGetIntegerv(GL_VIEWPORT, &data[0]);
		return { u32(data[2]), u32(data[3]) };
	}

	// glfwGetWindowMonitor only returns non-null for an exclusive-fullscreen window, so for
	// a windowed window we have to find whichever monitor it mostly overlaps instead.
	GLFWmonitor* getWindowMonitor(GLFWwindow* window) {
		if (GLFWmonitor* monitor = glfwGetWindowMonitor(window)) return monitor;

		i32 window_x, window_y, window_w, window_h;
		glfwGetWindowPos(window, &window_x, &window_y);
		glfwGetWindowSize(window, &window_w, &window_h);

		i32           monitor_count;
		GLFWmonitor** monitors     = glfwGetMonitors(&monitor_count);
		GLFWmonitor*  best_monitor = glfwGetPrimaryMonitor();
		i32           best_overlap = 0;

		for (i32 i = 0; i < monitor_count; ++i) {
			const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
			i32                monitor_x, monitor_y;
			glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

			i32 overlap_x = std::max(
				0,
				std::min(window_x + window_w, monitor_x + mode->width)
			        - std::max(window_x, monitor_x)
			);
			i32 overlap_y = std::max(
				0,
				std::min(window_y + window_h, monitor_y + mode->height)
			        - std::max(window_y, monitor_y)
			);
			i32 overlap = overlap_x * overlap_y;

			if (overlap > best_overlap) {
				best_overlap = overlap;
				best_monitor = monitors[i];
			}
		}

		return best_monitor;
	}
}

mk::Window::Window(u32 width, u32 height, std::string_view title) { create(width, height, title); }

mk::Window::Window(math::Vector2u size, std::string_view title) { create(size, title); }

void mk::Window::create(u32 width, u32 height, std::string_view title) {
	MK_ASSERT_TRUE(!initialized, "Called create on initalized window");

	window = mk::init::getWindow();
	glfw_to_mk_window.insert({ window, this });

	this->title = title;

	glfwSetWindowSize(window, width, height);
	glfwSetWindowTitle(window, title.begin());

	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
	glfwSetWindowContentScaleCallback(window, windowScaleFactorCallback);
	glfwSetKeyCallback(window, windowKeyCallback);
	glfwSetMouseButtonCallback(window, windowMouseButtonCallback);
	glfwSetCursorPosCallback(window, windowCursorPosCallback);
	glfwSetScrollCallback(window, windowScrollCallback);

	glfwGetWindowContentScale(window, &window_scale_factor.x, &window_scale_factor.y);
	MK_ASSERT_TRUE(window_scale_factor.x > 0, "Invalid window native x_scale");
	MK_ASSERT_TRUE(window_scale_factor.y > 0, "Invalid window native y_scale");

	glEnable(GL_DEPTH_TEST);

	std::cerr << " -- Requested window size: " << math::Vector2u{ width, height } << '\n';
	std::cerr << " -- Guessed window scaling factor: " << window_scale_factor << '\n';

	initialized = true;
	setSize(width, height);
	checkMonitorChange();
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
	glfwPollEvents();

	// This has to be after drawing - tested.
	if (frame_time_target > 0.f) {
		constexpr double spin_margin = 0.002;

		auto frame_deadline = last_frame_time + std::chrono::duration<double>(frame_time_target);
		auto spin_deadline  = frame_deadline - std::chrono::duration<double>(spin_margin);

		if (std::chrono::steady_clock::now() < spin_deadline)
			std::this_thread::sleep_until(spin_deadline);

		while (std::chrono::steady_clock::now() < frame_deadline) std::this_thread::yield();

		last_frame_time = std::chrono::steady_clock::now();
	}
}

void mk::Window::addEvent(Event event) {
	if (auto ev = event.get<Event::WindowScaleFactorChanged>())
		window_scale_factor = ev->scale_factors;
	else if (auto ev = event.get<Event::WindowResized>()) {
		ev->new_size = (ev->new_size.type<float>() / window_scale_factor).type<u32>();
		setSize(ev->new_size);
		event = *ev;
	} else if (event.is<Event::WindowClose>()) {
		setExitRequested(true);
	} else if (auto ev = event.get<Event::MouseMoved>()) {
		ev->new_position /= window_scale_factor;
		if (mouse_position != math::Vector2f{ -1.f })
			events.emplace(mk::Event::MouseMotionDelta(ev->new_position - mouse_position));
		mouse_position = ev->new_position;
	} else if (auto ev = event.get<Event::WindowMonitorChanged>()) {
		// Keep the frame limiter paced to whichever monitor the window is actually on now.
		if (frame_time_target > 0.f) setFrameRateLimit(ev->refresh_rate);
	}

	events.push(event);
}

std::optional<mk::Event> mk::Window::pollEvent() {
	if (!events.empty()) {
		auto event = events.front();
		events.pop();
		return event;
	}
	return std::nullopt;
}

void mk::Window::clear(Color color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

mk::math::Vector2u mk::Window::getSize() const { return window_size; }

void mk::Window::enableVerticalSync(bool enable) {
#ifdef __APPLE__
	if (enable) {
		GLFWmonitor*       monitor    = getWindowMonitor(window);
		const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
		setFrameRateLimit(u32(video_mode->refreshRate));
	} else
		setFrameRateLimit(0);
#else
	if (enable)
		glfwSwapInterval(1);

	else
		glfwSwapInterval(0);
#endif
}

void mk::Window::setFrameRateLimit(u32 fps) {
	frame_time_target = fps > 0 ? 1.f / float(fps) : 0.f;
	last_frame_time   = std::chrono::steady_clock::now();
}

void mk::Window::checkMonitorChange() {
	GLFWmonitor* monitor = getWindowMonitor(window);
	if (monitor == current_monitor) return;
	current_monitor = monitor;

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	addEvent(Event::WindowMonitorChanged{ u32(mode->refreshRate) });
}

void mk::Window::setFullscreen(bool enable) {
	if (enable == fullscreen) return;

	if (enable) {
		glfwGetWindowPos(window, &windowed_pos.x, &windowed_pos.y);
		windowed_size = window_size;

		GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		glfwSetWindowMonitor(
			window, nullptr, windowed_pos.x, windowed_pos.y, windowed_size.x, windowed_size.y, 0
		);
	}

	fullscreen = enable;
}

bool mk::Window::isFullscreen() const { return fullscreen; }

mk::math::Vector2f mk::Window::getScalingFactor() const { return window_scale_factor; }

bool mk::Window::isExitRequested() const { return exit_requested; }

void mk::Window::setExitRequested(bool value) { exit_requested = value; }

void mk::Window::setMouseCursorMode(const MouseMode mode) {
	mouse_mode = mode;
	switch (mode) {
	case MouseMode::HIDDEN:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case MouseMode::GRABBED:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	// case MouseMode::NORMAL:
	default:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

mk::Window::MouseMode mk::Window::getMouseCursorMode() const { return mouse_mode; }

void mk::Window::setRawMouseMotion(const bool enable) const {
	if (glfwRawMouseMotionSupported())
		if (enable)
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		else
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	else
		std::cerr << " -- Warning: Raw mouse motion not supported!" << '\n';
}

mk::Window::~Window() { glfwDestroyWindow(window); }

bool mk::Window::isKeyPressed(input::KEY key) const {
	return glfwGetKey(window, (i32) key) == GLFW_PRESS;
}

bool mk::Window::isMousePressed(input::MOUSE key) const {
	return glfwGetMouseButton(window, (i32) key) == GLFW_PRESS;
}

mk::math::Vector2i mk::Window::getMousePosition() const {
	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	return math::Vector2i(xpos, ypos);
}

GLFWwindow* mk::Window::getNativeHandle() const { return window; }
