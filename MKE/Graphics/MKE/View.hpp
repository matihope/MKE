#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/Math/Vector.hpp"

namespace mk {
	class View {
	public:
		virtual math::Matrix4f getTransform() const = 0;
		virtual ~View()                             = 0;
	};

	inline View::~View() {}

	class View2D: public View {
	public:
		View2D();
		~View2D() = default;
		View2D(math::Vector2f center, math::Vector2u size);

		math::Matrix4f getTransform() const override;

		void setSize(math::Vector2f size);

		void setCenter(math::Vector2f center);

	private:
		math::RectF view;
	};

	class View3D: public View {
	public:
		View3D();
		View3D(math::Vector3f position);

		~View3D() = default;

		math::Matrix4f getTransform() const override;

	private:
		math::Vector3f position;
	};
}
