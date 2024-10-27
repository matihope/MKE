#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow: public Window, public RenderTarget {
	public:
		using Window::Window;
		~RenderWindow() = default;

		void enableCamera2D(bool enable);

		void addEvent(Event event) override;

		void render(const Drawable& drawable) override;
		void renderContext(const Drawable& drawable, DrawContext context) override;

		void setCamera(math::Matrix4f camera) { camera_transform = camera; }

		math::Matrix4f getCamera() const;

		math::Vector2u getSize() const override;
		math::Vector2f getScalingFactor() const override;

	private:
		// Whether or not enable automatic RenderWindow's camera 2d features:
		// - automatic scaling on resize and scale factor modification
		bool           enable_camera_2d = false;
		math::Matrix4f camera_transform{};

		void updateCamera2D();
	};
}
