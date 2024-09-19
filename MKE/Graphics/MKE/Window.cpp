#include "Window.hpp"
#include "MKE/Event.hpp"
#include "MKE/Panic.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>

#include <unordered_map>

namespace {
	void mk_initGlfw() {
		static bool initialized_glfw = false;
		if (!initialized_glfw) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
		event.window_resized = mk::Events::WindowResized({ width, height });
		pushEvent(window, event);
	}
}

mk::Window::Window(u32 width, u32 height, std::string_view title) { create(width, height, title); }

mk::Window::Window(math::Vector2u size, std::string_view title) { create(size, title); }

mk::Window::~Window() {
	// This generates a problem if there were more than 2 windows opened...
	if (initialized) glfwTerminate();
}

void mk::Window::create(uint32_t width, uint32_t height, std::string_view title) {
	MKE_ASSERT_TRUE(!initialized, "Called create on initalized window");

	this->title = title;
	setSize(width, height);

	mk_initGlfw();

	window = glfwCreateWindow(window_size.x, window_size.y, this->title.c_str(), NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		MKE_PANIC("Coulnd't create GLFW window");
	}
	glfw_to_mk_window.insert({ window, (*this) });
	glfwMakeContextCurrent(window);

	MKE_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD");

	glViewport(0, 0, window_size.x, window_size.y);

	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);

	initialized = true;
}

void mk::Window::create(math::Vector2u size, std::string_view title) {
	create(size.x, size.y, title);
}

void mk::Window::setSize(math::Vector2u size) {
	window_size = size;
	if (initialized) {
		//...
	}
}

void mk::Window::setSize(unsigned int width, unsigned int height) { setSize({ width, height }); }

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
	glClearColor(color.r / 255.f, color.g / 255.f, color.g / 255.f, color.a / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}
