#include "Camera.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"

void mk::Camera2D::update(Game& game, const float dt) {
	WorldEntity2D::update(game, dt);
	game.getRenderWindow().setView2D(view);
}

void mk::Camera3D::update(Game& game, const float dt) {
	WorldEntity3D::update(game, dt);
	game.getRenderWindow().setView3D(view);
}

void mk::Camera3D::setPosition(math::Vector3f position) {
	WorldEntity3D::setPosition(position);
	view.setPosition(position);
}

void mk::Camera3D::setDirection(math::Vector3f direction) { view.setDirection(direction); }

void mk::Camera3D::lookAt(math::Vector3f at) { view.lookAt(at); }

mk::math::Vector3f mk::Camera3D::getDirection() const { return view.getDirection(); }

mk::math::Vector3f mk::Camera3D::getPitchYawRoll() const { return view.getPithYawRoll(); }

void mk::Camera3D::setFov(float fov) { view.setFovH(fov); }

void mk::Camera3D::setNear(float near) { view.setNear(near); }

void mk::Camera3D::setFar(float far) { view.setFar(far); }

void mk::Camera3D::setAspect(float aspect) { view.setAspect(aspect); }

float mk::Camera3D::getFovH() const { return view.getFovH(); }

float mk::Camera3D::getFovV() const { return view.getFovV(); }

void mk::Camera3D::setPitchYawRoll(math::Vector3f pitch_yaw_roll) {
	view.setPithYawRoll(pitch_yaw_roll);
}

void mk::Camera3D::event(Game& game, const Event& event) {
	if (const auto ev = event.get<Event::WindowResized>(); ev) {
		auto [w, h] = ev->new_size.type<float>().vec_data;
		view.setAspect(w / h);
	}
	WorldEntity3D::event(game, event);
}
