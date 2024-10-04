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
