#include "Transformable.hpp"
#include "MKE/Math/Vector.hpp"

void mk::Transformable::setOrigin(math::Vector2f origin) { setOrigin(origin.x, origin.y); }

void mk::Transformable::setOrigin(math::Vector3f origin) { this->origin = origin; }

void mk::Transformable::setOrigin(float x, float y) { setOrigin({ x, y, origin.z }); }

mk::math::Vector3f mk::Transformable::getOrigin() const { return origin; }

void mk::Transformable::setOrigin(float x, float y, float z) { setOrigin({ x, y, z }); }

void mk::Transformable::setPosition(math::Vector2f position) {
	setPosition(position.x, position.y);
}

void mk::Transformable::setPosition(math::Vector3f position) { this->position = position; }

void mk::Transformable::setPosition(float x, float y) { setPosition(x, y, position.z); }

void mk::Transformable::setPosition(float x, float y, float z) { setPosition({ x, y, z }); }

void mk::Transformable::move(math::Vector2f delta) { move(delta.x, delta.y); }

void mk::Transformable::move(math::Vector3f delta) { position += delta; }

void mk::Transformable::move(float delta_x, float delta_y) {
	move(math::Vector3f(delta_x, delta_y, 0));
}

void mk::Transformable::rotate(const math::Vector3f delta) { rotation += delta; }

void mk::Transformable::move(float delta_x, float delta_y, float delta_z) {
	move({ delta_x, delta_y, delta_z });
}

mk::math::Vector2f mk::Transformable::getPosition2D() const { return { position.x, position.y }; }

mk::math::Vector3f mk::Transformable::getPosition() const { return position; }

void mk::Transformable::setRotation(math::Vector3f rotation) { this->rotation = rotation; }

void mk::Transformable::rotate(float d_pitch, float d_yaw, float d_roll) {
	rotate({ d_pitch, d_yaw, d_roll });
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

void mk::Transformable::setScale(math::Vector3f scale) { this->scale = scale; }

void mk::Transformable::setScale(float xscale, float yscale, float zscale) {
	setScale({ xscale, yscale, zscale });
}

void mk::Transformable::setRotation(float d_pitch, float d_yaw, float d_roll) {
	setRotation({ d_pitch, d_yaw, d_roll });
}

mk::math::Matrix4f mk::Transformable::getTransform() const {
	math::Matrix4f scale_matrix{ 1 };
	scale_matrix(0, 0) = scale.x;
	scale_matrix(1, 1) = scale.y;
	scale_matrix(2, 2) = scale.z;

	math::Matrix4f origin_matrix{ 1 };
	origin_matrix(0, 3) = -origin.x;
	origin_matrix(1, 3) = -origin.y;
	origin_matrix(2, 3) = -origin.z;

	math::Matrix4f position_matrix{ 1 };
	position_matrix(0, 3) = position.x;
	position_matrix(1, 3) = position.y;
	position_matrix(2, 3) = position.z;

	return position_matrix * getRotationTransform() * scale_matrix * origin_matrix;
}

void mk::Transformable::rotate(float d_pitch) { rotation.x += d_pitch; }
