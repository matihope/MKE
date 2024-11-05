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

void mk::Camera3D::event(Game& game, const Event& event) {
	if (event.type == EventType::WindowResized) {
		auto [w, h] = event.window_resized.new_size.type<float>().bind();
		view.setAspect(w / h);
	}
	WorldEntity3D::event(game, event);
}
