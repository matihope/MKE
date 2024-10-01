#pragma once

#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/VertexArray.hpp"

namespace mk {
	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget() = 0;

		void render(const Drawable& drawable) const;

		void render(const Drawable2D& drawable, DrawContext2D context = DrawContext2D()) const;

		void render(const VertexArray2D& vertex_array, DrawContext2D context) const;

	private:
	};

	inline RenderTarget::~RenderTarget() {};
}
