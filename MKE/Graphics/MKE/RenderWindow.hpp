#pragma once

#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow: public Window, public RenderTarget {
	public:
		using Window::Window;
        ~RenderWindow() = default;
	};
}
