#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow: public Window, public RenderTarget {
	public:
		using Window::Window;
		~RenderWindow() = default;

		void render(const Drawable& drawable, DrawContext context) override;

		math::Vector2u getSize() const override;
		math::Vector2f getScalingFactor() const override;
	};
}
