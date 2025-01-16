#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"

namespace mk {
	class Transformable {
	public:
		virtual ~Transformable() {}

		math::Vector2f getPosition2D() const;

		virtual void setOrigin(math::Vector3f origin);
		void         setOrigin(math::Vector2f origin);
		void         setOrigin(float x, float y, float z);
		void         setOrigin(float x, float y);

		virtual void setPosition(math::Vector3f position);  // MAIN
		void         setPosition(math::Vector2f position);
		void         setPosition(float x, float y, float z);
		void         setPosition(float x, float y);

		math::Vector3f getPosition() const;

		virtual void move(math::Vector3f delta);  // MAIN
		void move(math::Vector2f delta);
		void move(float delta_x, float delta_y, float delta_z);
		void move(float delta_x, float delta_y);

		virtual void rotate(math::Vector3f delta);  // MAIN
		void         rotate(float d_pitch, float d_yaw, float d_roll);
		void         rotate(float d_pitch);

		virtual void setRotation(math::Vector3f rotation);  // MAIN
		void setRotation(float d_pitch, float d_yaw, float d_roll);

		virtual void setScale(math::Vector3f scale); // MAIN
		void setScale(float xscale, float yscale, float zscale);

		math::Matrix4f getTransform() const;
		math::Matrix4f getRotationTransform() const;

	private:
		math::Vector3f position;
		math::Vector3f origin;
		math::Vector3f rotation;
		math::Vector3f scale{ 1 };
	};

	class DummyTransformable: public Transformable {};
}
