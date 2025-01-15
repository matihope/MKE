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

mk::math::Matrix4f mk::View3D::getTransform() const { return camera; }

mk::View3D::View3D(): mk::View3D(math::Vector3f(0.f)) {}

void mk::View3D::lookAt(math::Vector3f look_at) {
	direction = (look_at - position).normalizeOrZero();
	makeCamera();
}

void mk::View3D::makeCamera() {
	// camera = math::perspective(45.f, 16.f / 9, 0.1f, 100.f);
	// camera = math::lookAtDirection(position, direction, math::Vector3f(0.f, 1.f, 0.f));
	camera = math::perspective(fov, aspect, 0.1f, 1000.f);
	camera *= lookAtDirection(position, direction, { 0.f, 1.f, 0.f });
	// camera = math::orthogonal(16, 9);
}

void mk::View3D::setPosition(math::Vector3f position) {
	if (this->position != position) {
		this->position = position;
		makeCamera();
	}
}

mk::math::Vector3f mk::View3D::getPosition() const { return position; }

mk::View3D::View3D(math::Vector3f position, math::Vector3f direction) {
	setPosition(position);
	lookAt(position + direction);
}

void mk::View3D::setDirection(math::Vector3f direction) {
	this->direction = direction;
	makeCamera();
}

mk::math::Vector3f mk::View3D::getDirection() const { return direction; }

void mk::View3D::setFov(float fov) {
	this->fov = fov;
	makeCamera();
}

void mk::View3D::setAspect(float aspect) {
	this->aspect = aspect;
	makeCamera();
}

mk::math::Vector2f mk::View2D::getSize() const { return view.getSize(); }
