#include "Transformable.hpp"

void mk::Transformable::setPosition(math::Vector2f position) {
	setPosition(position.x, position.y);
}

void mk::Transformable::setPosition(math::Vector3f position) {
	setPosition(position.x, position.y, position.z);
}

void mk::Transformable::setPosition(float x, float y) {
	transform(0, 3) = x;
	transform(1, 3) = y;
}

void mk::Transformable::setPosition(float x, float y, float z) {
	transform(0, 3) = x;
	transform(1, 3) = y;
	transform(2, 3) = z;
}

void mk::Transformable::move(math::Vector2f delta) { move(delta.x, delta.y); }

void mk::Transformable::move(math::Vector3f delta) { move(delta.x, delta.y, delta.z); }

void mk::Transformable::move(float delta_x, float delta_y) {
	transform(0, 3) += delta_x;
	transform(1, 3) += delta_y;
}

void mk::Transformable::move(float delta_x, float delta_y, float delta_z) {
	transform(0, 3) += delta_x;
	transform(1, 3) += delta_y;
	transform(2, 3) += delta_z;
}

mk::math::Vector2f mk::Transformable::getPosition2D() const {
	return { transform(0, 3), transform(1, 3) };
}

mk::math::Vector3f mk::Transformable::getPosition3D() const {
	return { transform(0, 3), transform(1, 3), transform(2, 3) };
}

void mk::Transformable::setRotation(math::Vector3f rotation) { this->rotation = rotation; }

void mk::Transformable::rotate(float d_pitch, float d_yaw, float d_roll) {
	rotation.x += d_pitch;
	rotation.y += d_yaw;
	rotation.z += d_roll;
}

mk::math::Matrix4f mk::Transformable::getRotationTransform() const {
	using std::cos;
	using std::sin;
	float          a = rotation.x;
	float          b = rotation.y;
	float          c = rotation.z;
	math::Matrix4f result;
	result(0, 0) = cos(a) * cos(b);
	result(0, 1) = cos(a) * sin(b) * sin(c) - sin(a) * cos(c);
	result(0, 2) = cos(a) * sin(b) * cos(c) + sin(a) * sin(c);
	result(1, 0) = sin(a) * cos(b);
	result(1, 1) = sin(a) * sin(b) * sin(c) + cos(a) * cos(c);
	result(1, 2) = sin(a) * sin(b) * cos(c) - cos(a) * sin(c);
	result(2, 0) = -sin(b);
	result(2, 1) = cos(b) * sin(c);
	result(2, 2) = cos(b) * cos(c);
	return result;
}

void mk::Transformable::setScale(float xscale, float yscale, float zscale) {
	transform(0, 0) = xscale;
	transform(1, 1) = yscale;
	transform(2, 2) = zscale;
}
