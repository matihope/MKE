#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"

namespace mk {
	class Transformable {
	public:
		virtual ~Transformable() {};

		constexpr math::Matrix4f getTransform() const { return transform * getRotationTransform(); }

		math::Vector2f getPosition2D() const;
		math::Vector3f getPosition3D() const;

		void setPosition(math::Vector2f position);
		void setPosition(math::Vector3f position);
		void setPosition(float x, float y);
		void setPosition(float x, float y, float z);

		void move(math::Vector2f delta);
		void move(math::Vector3f delta);
		void move(float delta_x, float delta_y);
		void move(float delta_x, float delta_y, float delta_z);

		void setRotation(math::Vector3f rotation);

		void rotate(float d_pitch, float d_yaw, float d_roll);

		void setScale(float xscale, float yscale, float zscale);

		math::Matrix4f getRotationTransform() const;

	private:

		math::Vector3f position;
		math::Vector3f rotation;
		math::Vector3f scale{ 1 };
		math::Matrix4f transform{ 1 };
	};
}
