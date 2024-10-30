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

		void setView2D(const View2D& view2d) { custom_view2d = view2d; }

		void setView3D(const View3D& view3d) { custom_view3d = view3d; }

		const View2D& getCurrentView2D() const;
		const View3D& getCurrentView3D() const;

		const View2D& getDefaultView2D() const {
			static View2D view({ 0, 0 }, { 1, 1 });
			view.setSize(getSize().type<float>());
			view.setCenter(getSize().type<float>() / 2.f);
			return view;
		}

		const View3D& getDefaultView3D() const {
			static View3D view(math::Vector3f{ 0.f });
			return view;
		}

		math::Vector2f mapPixelToCoords2D(const math::Vector2i& point) const;

	protected:
		void defaultRender(const Drawable& drawable, DrawContext context);

	private:
		std::optional<View2D> custom_view2d;
		std::optional<View3D> custom_view3d;
	};

	inline RenderTarget::~RenderTarget() {}
}
