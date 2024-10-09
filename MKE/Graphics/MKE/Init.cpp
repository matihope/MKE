#include "Init.hpp"
#include "MKE/Panic.hpp"
#include <GLFW/glfw3.h>

namespace {
	struct MkeInit {
		MkeInit() {
			static bool initialized_glfw = false;
			if (!initialized_glfw) {
				std::cerr << "-- Initializing MKE...\n";
				MK_ASSERT(glfwInit(), "Couldn\'t init glfw");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
				initialized_glfw = true;
			}
		}

		~MkeInit() {
			std::cerr << "-- Deinitializing MKE...\n";
			glfwTerminate();
		}
	};
}

void mk::init() { static MkeInit mke_init; }
