#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Window.hpp"

namespace mk {
	class RenderWindow final: public Window, public RenderTarget {
	public:
		using Window::Window;
		~RenderWindow() override = default;

		void render(const Drawable& drawable, DrawContext context) override;

		[[nodiscard]]
		math::Vector2u getSize() const override;
		[[nodiscard]]
		math::Vector2f getScalingFactor() const override;
	};
}
