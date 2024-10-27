#pragma once

#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"

namespace mk {
	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget()                                                           = 0;
		virtual void render(const Drawable& drawable) const                             = 0;
		virtual void renderContext(const Drawable& drawable, DrawContext context) const = 0;

	protected:
		void defaultRender(const Drawable& drawable, DrawContext context) const;
	};

	inline RenderTarget::~RenderTarget() {};
}
