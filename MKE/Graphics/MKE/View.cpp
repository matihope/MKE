#include "View.hpp"
#include "MKE/Math/MatrixUtils.hpp"
#include "MKE/Transformable.hpp"

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

mk::math::Matrix4f mk::View3D::getTransform() const {
	return math::perspective(45, 16.f / 9, 0.05f, 100.f);
}

mk::View3D::View3D(): mk::View3D(math::Vector3f(0.f)) {}
