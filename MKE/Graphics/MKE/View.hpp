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

		math::Vector2f getSize() const;

		void setCenter(math::Vector2f center);

	private:
		math::RectF view;
	};

	class View3D: public View {
	public:
		View3D();
		View3D(math::Vector3f position);
		View3D(math::Vector3f position, math::Vector3f direction);

		~View3D() = default;

		void           lookAt(math::Vector3f look_at);
		math::Matrix4f getTransform() const override;

		void           setPosition(math::Vector3f position);
		math::Vector3f getPosition() const;

		void           setDirection(math::Vector3f direction);
		math::Vector3f getDirection() const;

		void setFov(float fov);
		void setAspect(float aspect);
		void setNear(float near);
		void setFar(float far);

		[[nodiscard]]
		math::Vector3f getPithYawRoll() const;
		void           setPithYawRoll(math::Vector3f pitch_yaw_roll);


	private:
		float fov    = 45.f;
		float aspect = 1.f;
		float near   = 0.1;
		float far    = 1000.f;

		void makeCamera();

		math::Vector3f position;
		math::Vector3f pitch_yaw_roll;
		math::Matrix4f camera;
	};
}
