#include "View.hpp"
#include "MKE/Math/MatrixUtils.hpp"
#include "MKE/Math/Vector.hpp"

mk::View2D::View2D(): view(0, 0, 2, 2) {}

mk::View2D::View2D(math::Vector2f center, math::Vector2u size):
	  view(center.x, center.y, size.x, size.y) {}

mk::math::Matrix4f mk::View2D::getTransform() const {
	return math::camera2D(view.width, view.height);
}

void mk::View2D::setSize(math::Vector2f size) {
	view.width  = size.x;
	view.height = size.y;
}

void mk::View2D::setCenter(math::Vector2f center) {
	view.left = center.x;
	view.top  = center.y;
}

mk::View3D::View3D(math::Vector3f position): position(position) {}

mk::View3D::View3D(): mk::View3D(math::Vector3f(0.f)) {}

mk::View3D::View3D(math::Vector3f position, math::Vector3f direction) {
	setPosition(position);
	lookAt(position + direction);
}

mk::math::Matrix4f mk::View3D::getTransform() const { return camera; }

void mk::View3D::lookAt(math::Vector3f look_at) {
	setDirection((look_at - position).normalizeOrZero());
	makeCamera();
}

void mk::View3D::makeCamera() {
	// camera = math::perspective(45.f, 16.f / 9, 0.1f, 100.f);
	// camera = math::lookAtDirection(position, direction, math::Vector3f(0.f, 1.f, 0.f));
	camera = math::perspective(fov_h, aspect, near, far);
	camera *= lookAtDirection(position, getDirection(), { 0.f, 1.f, 0.f });
	// camera = math::orthogonal(16, 9);
}

void mk::View3D::setPosition(math::Vector3f position) {
	if (this->position != position) {
		this->position = position;
		makeCamera();
	}
}

mk::math::Vector3f mk::View3D::getPosition() const { return position; }

void mk::View3D::setDirection(const math::Vector3f direction) {
	auto [x, y, z]       = direction.vec_data;
	this->pitch_yaw_roll = { std::asin(y), atan2(z, x), 0.f };
	makeCamera();
}

mk::math::Vector3f mk::View3D::getDirection() const {
	const auto pitch_sin = std::sin(pitch_yaw_roll.x);
	const auto pitch_cos = std::cos(pitch_yaw_roll.x);
	const auto yaw_sin   = std::sin(pitch_yaw_roll.y);
	const auto yaw_cos   = std::cos(pitch_yaw_roll.y);
	return { pitch_cos * yaw_cos, pitch_sin, pitch_cos * yaw_sin };
}

void mk::View3D::setFovH(const float fov) {
	this->fov_h = fov;
	makeCamera();
}

float mk::View3D::getFovH() const { return fov_h; }

float mk::View3D::getFovV() const { return fov_h / aspect; }

void mk::View3D::setAspect(const float aspect) {
	this->aspect = aspect;
	makeCamera();
}

void mk::View3D::setNear(const float near) {
	this->near = near;
	makeCamera();
}

void mk::View3D::setFar(const float far) {
	this->far = far;
	makeCamera();
}

mk::math::Vector3f mk::View3D::getPithYawRoll() const { return pitch_yaw_roll; }

void mk::View3D::setPithYawRoll(math::Vector3f pitch_yaw_roll) {
	auto [x, y, z]       = pitch_yaw_roll.vec_data;
	x                    = std::fmodf(x, 2. * M_PI);
	y                    = std::fmodf(y, 2. * M_PI);
	z                    = std::fmodf(z, 2. * M_PI);
	this->pitch_yaw_roll = { x, y, z };
	makeCamera();
}

float mk::View3D::getAspect() const { return aspect; }

mk::math::Vector2f mk::View2D::getSize() const { return view.getSize(); }
