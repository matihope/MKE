#pragma once

#include "MKE/DrawContext.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Texture.hpp"

namespace mk {
	// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
	class RenderTexture2D: public RenderTarget2D {
	public:
		RenderTexture2D();

		~RenderTexture2D();

		const Texture& getTexture() const;
		void           setSmooth(bool smooth);

		void setScalingFactor(math::Vector2f scaling_factor);

		u32 getFBO() const;

		void create(usize width, usize height);
		void create(math::Vector2u size);

		void render2d(const Drawable2D& drawable) const override;
		void render2dContext(const Drawable2D& drawable, DrawContext context) const override;

		void clear(mk::Color color) const;

	private:
		void           regenerateBuffers();
		math::Vector2u size{};
		math::Vector2f scaling_factor{ 1.f };  // viewport size = size * scaling_factor;
		Texture        texture;
		u32            frame_buffer_id{};
		u32            render_buffer_object{};
		math::Matrix4f camera_transform{ 1.f };
	};
}
