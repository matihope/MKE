#include "Player.hpp"

constexpr float SAFE_FRAC_PI_2 = M_PI_2 - 0.0001;

void Player::onReady(mk::Game& game) {
	camera = addChild<mk::Camera3D>(game);
	setPosition({ 64.f, 64.f, 64.f });
	camera->setPosition(getPosition());
	camera->lookAt({ 16.f, 32.f, 16.f });
	camera->setFov(60.f);
}

void Player::onUpdate(mk::Game& game, const float dt) {
	// Rotate camera
	if (game.getRenderWindow().getMouseCursorMode() == mk::Window::MouseMode::GRABBED) {
		auto [mouse_dx, mouse_dy] = game.getMouseDelta().bind();
		auto [pitch, yaw, roll]   = camera->getPitchYawRoll().bind();
		yaw += mouse_dx * MOUSE_SENSITIVITY * dt;
		pitch += -mouse_dy * MOUSE_SENSITIVITY * dt;

		// Keep the camera's angle from going too high/low.
		if (pitch < -SAFE_FRAC_PI_2)
			pitch = -SAFE_FRAC_PI_2;
		else if (pitch > SAFE_FRAC_PI_2)
			pitch = SAFE_FRAC_PI_2;
		camera->setPitchYawRoll({ pitch, yaw, roll });
	}

	// Move the player
	using namespace mk::input;

	mk::math::Vector3f player_input;
	player_input.z = game.isKeyPressed(KEY::W) - game.isKeyPressed(KEY::S);
	player_input.x = game.isKeyPressed(KEY::D) - game.isKeyPressed(KEY::A);
	player_input.y = game.isKeyPressed(KEY::SPACE) - game.isKeyPressed(KEY::LEFT_SHIFT);
	player_input   = player_input.normalizeOrZero() * PLAYER_SPEED;
	if (game.isKeyPressed(KEY::LEFT_CONTROL)) player_input *= PLAYER_BOOST;

	auto forward = camera->getDirection();
	forward.y    = 0;
	forward      = forward.normalizeOrZero();
	auto right   = -cross(mk::math::Vector3fUP, forward).normalizeOrZero();

	auto move_dir = forward * player_input.z + right * player_input.x;
	move_vec      = move_vec.lerp(move_dir, dt * PLAYER_LERP_SPEED, 1e-4f);
	move((move_vec + mk::math::Vector3fUP * player_input.y) * dt);
	camera->setPosition(getPosition());
}
