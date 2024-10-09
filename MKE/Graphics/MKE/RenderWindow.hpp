#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow: public Window, public RenderTarget2D, public RenderTarget3D {
	public:
		using Window::Window;
		~RenderWindow() = default;

		void enableCamera2D(bool enable);

		void addEvent(Event event) override;

		void render2d(const Drawable2D& drawable) const {
			DrawContext context;
			context.camera = camera_transform;
			render2d(drawable, context);
		}

		void render2d(const Drawable2D& drawable, DrawContext context) const override {
			RenderTarget2D::render2d(drawable, context);
		}

		void render3d(const Drawable3D& drawable) const {
			DrawContext context;
			context.camera = camera_transform;
			render3d(drawable, context);
		}

		void render3d(const Drawable3D& drawable, DrawContext context) const override {
			RenderTarget3D::render3d(drawable, context);
		}

		void setCamera(math::Matrix4f camera) { camera_transform = camera; }

	private:
		// Whether or not enable automatic RenderWindow's camera 2d features:
		// - automatic scaling on resize and scale factor modification
		bool           enable_camera_2d = false;
		math::Matrix4f camera_transform{};

		void updateCamera2D();
	};
}
