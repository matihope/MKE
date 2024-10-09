#pragma once

#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"

namespace mk {
	class RenderTarget2D: public NonCopyable {
	public:
		virtual ~RenderTarget2D()                               = 0;
		virtual void render2d(const Drawable2D& drawable) const = 0;
		virtual void render2d(const Drawable2D& drawable, DrawContext context) const;
	};

	inline RenderTarget2D::~RenderTarget2D() {};

	class RenderTarget3D: public NonCopyable {
	public:
		virtual ~RenderTarget3D()                               = 0;
		virtual void render3d(const Drawable3D& drawable) const = 0;
		virtual void render3d(const Drawable3D& drawable, DrawContext context) const;
	};

	inline RenderTarget3D::~RenderTarget3D() {};
}
