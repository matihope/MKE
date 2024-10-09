#pragma once

#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"

namespace mk {
	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget() = 0;

		virtual void render(const Drawable& drawable) const;
	};

	inline RenderTarget::~RenderTarget() {};

	class RenderTarget2D: public RenderTarget {
	public:
		virtual void render2d(const Drawable2D& drawable, DrawContext context) const;
	};

	class RenderTarget3D: public RenderTarget {
	public:
		virtual void render3d(const Drawable3D& drawable, DrawContext context) const;
	};
}
