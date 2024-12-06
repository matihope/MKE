#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/View.hpp"

namespace mk {
	class RenderTarget: public NonCopyable {
	public:
		virtual ~RenderTarget()                                            = 0;
		virtual void render(const Drawable& drawable, DrawContext context) = 0;

		virtual math::Vector2u getSize() const          = 0;
		virtual math::Vector2f getScalingFactor() const = 0;

		void setView2D(const View2D& view2d);
		void setView3D(const View3D& view3d);

		const View2D& getCurrentView2D() const;
		const View3D& getCurrentView3D() const;

		const View2D& getDefaultView2D() const;
		const View3D& getDefaultView3D() const;

		math::Vector2f mapPixelToCoords2D(const math::Vector2i& point) const;

	protected:
		void defaultRender(const Drawable& drawable, DrawContext context);

	private:
		std::optional<View2D> custom_view2d;
		std::optional<View3D> custom_view3d;
	};

	inline RenderTarget::~RenderTarget() {}
}
