#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"

namespace mk {
	class Transformable {
	public:
		virtual ~Transformable() {}

		math::Vector2f getPosition2D() const;

		void setOrigin(math::Vector3f origin);
		void setOrigin(math::Vector2f origin);
		void setOrigin(float x, float y, float z);
		void setOrigin(float x, float y);

		[[nodiscard]]
		math::Vector3f getOrigin() const;

		void setPosition(math::Vector3f position);  // MAIN
		void setPosition(math::Vector2f position);
		void setPosition(float x, float y, float z);
		void setPosition(float x, float y);

		[[nodiscard]]
		math::Vector3f getPosition() const;

		void move(math::Vector3f delta);
		void move(math::Vector2f delta);
		void move(float delta_x, float delta_y, float delta_z);
		void move(float delta_x, float delta_y);

		void rotate(math::Vector3f delta);
		void rotate(float d_pitch, float d_yaw, float d_roll);
		void rotate(float d_pitch);

		void setRotation(math::Vector3f rotation);
		void setRotation(float d_pitch, float d_yaw, float d_roll);

		void setScale(math::Vector3f scale);
		void setScale(float xscale, float yscale, float zscale);

		math::Vector3f getScale() const;

		math::Matrix4f getTransform() const;
		math::Matrix4f getRotationTransform() const;

	private:
		math::Vector3f position;
		math::Vector3f origin;
		math::Vector3f rotation;
		math::Vector3f scale{ 1 };

	protected:
		virtual void setOriginBase(math::Vector3f origin);
		virtual void setPositionBase(math::Vector3f position);
		virtual void moveBase(math::Vector3f delta);
		virtual void rotateBase(math::Vector3f delta);
		virtual void setRotationBase(math::Vector3f rotation);
		virtual void setScaleBase(math::Vector3f scale);
	};

	class DummyTransformable: public Transformable {};
}
