#include "Window.hpp"
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Panic.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>

#include <unordered_map>

namespace {
	void mk_initGlfw() {
		static bool initialized_glfw = false;
		if (!initialized_glfw) {
			MK_ASSERT(glfwInit(), "Couldn\'t init glfw");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
			initialized_glfw = true;
		}
	}

	std::unordered_map<GLFWwindow*, std::reference_wrapper<mk::Window>> glfw_to_mk_window{};

	void pushEvent(GLFWwindow* window, mk::Event event) {
		glfw_to_mk_window.at(window).get().addEvent(event);
	}

	void window_close_callback(GLFWwindow* window) {
		mk::Event event{};
		event.window_close = mk::Events::WindowClose();
		pushEvent(window, event);
	}

	void window_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		mk::Event event{};
		event.window_resized = mk::Events::WindowResized(mk::math::Vector2u(width, height));
		pushEvent(window, event);
	}

	void window_key_callback(
		GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */
	) {
		mk::Event event;

		// This is fine, because mk::input::KEY follows GLFW's key ordering.
		if (action == GLFW_PRESS)
			event.key_pressed = mk::Events::KeyPressed(mk::input::KEY(key));
		else if (action == GLFW_RELEASE)
			event.key_released = mk::Events::KeyReleased(mk::input::KEY(key));

		pushEvent(window, event);
	}

	void window_mouse_callback(
		GLFWwindow* /* window */, int /* button */, int /* action */, int /* mods */
	) {}

	mk::math::Vector2u getGlViewportSize() {
		i32 data[4];
		glGetIntegerv(GL_VIEWPORT, &data[0]);
		return { u32(data[2]), u32(data[3]) };
	}
}

mk::Window::Window(u32 width, u32 height, std::string_view title) { create(width, height, title); }

mk::Window::Window(math::Vector2u size, std::string_view title) { create(size, title); }

mk::Window::~Window() {
	// This generates a problem if there were more than 2 windows opened...
	if (initialized) glfwTerminate();
}

void mk::Window::create(u32 width, u32 height, std::string_view title) {
	MK_ASSERT_TRUE(!initialized, "Called create on initalized window");

	mk_initGlfw();

	this->title = title;
	window      = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		MK_PANIC("Couldn't create GLFW window");
	}

	glfw_to_mk_window.insert({ window, (*this) });
	glfwMakeContextCurrent(window);

	MK_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD");

	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
	glfwSetKeyCallback(window, window_key_callback);
	glfwSetMouseButtonCallback(window, window_mouse_callback);

	initialized = true;
	setSize(getGlViewportSize());
}

void mk::Window::create(math::Vector2u size, std::string_view title) {
	create(size.x, size.y, title);
}

void mk::Window::setSize(math::Vector2u size) {
	window_size = size;
	if (initialized) glViewport(0, 0, size.x, size.y);
}

void mk::Window::setSize(u32 width, u32 height) { setSize({ width, height }); }

void mk::Window::display() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void mk::Window::addEvent(Event event) {
	if (event.type == mk::EventType::WindowResized) setSize(event.window_resized.new_size);
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
	glClear(GL_COLOR_BUFFER_BIT);
}

mk::math::Vector2u mk::Window::getSize() const { return window_size; }

bool mk::Window::isKeyPressed(input::KEY key) const {
	return glfwGetKey(window, (i32) key) == GLFW_PRESS;
}

void mk::Window::enableVerticalSync(bool enable) {
	if (enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}
