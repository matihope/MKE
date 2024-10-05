#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"

namespace mk {
	class Transformable {
	public:
		virtual ~Transformable() {};

		constexpr math::Matrix4f& getTransform() { return transform; }

		constexpr const math::Matrix4f& getTransform() const { return transform; }

		math::Vector2f getPosition() const;

		void setPosition(math::Vector2f position);
		void setPosition(math::Vector3f position);
		void setPosition(float x, float y);
		void setPosition(float x, float y, float z);

		void move(math::Vector2f delta);
		void move(math::Vector3f delta);
		void move(float delta_x, float delta_y);
		void move(float delta_x, float delta_y, float delta_z);

	private:
		math::Matrix4f transform{};
	};
}
