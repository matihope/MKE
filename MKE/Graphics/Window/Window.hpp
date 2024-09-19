#pragma once

#include <MKE/math/Vector2.hpp>

#include <cstdint>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mk {
	class Window {
	public:
		Window() {};
		Window(uint32_t width, uint32_t height){};

	private:
		math::Vector2u window_size;
    };
}
