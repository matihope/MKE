#include "Init.hpp"
#include "MKE/Panic.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace {
	struct MkeInit {
		GLFWwindow* window;

		MkeInit() {
			static bool initialized_mk = false;
			if (!initialized_mk) {
				std::cerr << "-- Initializing MKE...\n";
				MK_ASSERT(glfwInit(), "Couldn\'t init glfw");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
				initialized_mk = true;

				window = glfwCreateWindow(1, 1, "MKE_Window", NULL, NULL);
				if (window == NULL) {
					glfwTerminate();
					MK_PANIC("Couldn\'t create GLFW window");
				}

				glfwMakeContextCurrent(window);

				MK_ASSERT(
					gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD"
				);
			}
		}

		~MkeInit() {
			std::cerr << "-- Deinitializing MKE...\n";
			glfwTerminate();
		}
	};

	MkeInit& instance() {
		static MkeInit mke_init;
		return mke_init;
	}
}

void mk::init::init() { instance(); }

GLFWwindow* mk::init::getWindow() { return instance().window; }
