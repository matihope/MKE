#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow: public Window, public RenderTarget {
	public:
		using Window::Window;
		~RenderWindow() = default;

		void enableCamera2D(bool enable);

		void addEvent(Event event) override;


	private:
		// Whether or not enable automatic RenderWindow's camera 2d features:
		// - automatic scaling on resize and scale factor modification
		bool           enable_camera_2d = false;
		math::Matrix4f camera_transform{};

		void updateCamera();
	};
}
