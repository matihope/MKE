#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"

namespace mk {
	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget()                                                   = 0;
		virtual void render(const Drawable& drawable)                             = 0;
		virtual void renderContext(const Drawable& drawable, DrawContext context) = 0;

		virtual math::Vector2u getSize() const          = 0;
		virtual math::Vector2f getScalingFactor() const = 0;

	protected:
		void defaultRender(const Drawable& drawable, DrawContext context);
	};

	inline RenderTarget::~RenderTarget() {}
}
