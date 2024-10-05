#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"

namespace mk {
	class VertexArray2D;

	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget() = 0;

		void render(const Drawable& drawable) const;

		void render(const Drawable2D& drawable, DrawContext2D context = DrawContext2D()) const;

		void render(const VertexArray2D& vertex_array, DrawContext2D context) const;

		void setCamera(math::Matrix4f camera_transform);

	private:
		math::Matrix4f camera_transform {};
	};

	inline RenderTarget::~RenderTarget() {};
}
