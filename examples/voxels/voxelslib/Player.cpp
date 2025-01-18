#include "Player.hpp"

#include "World.hpp"
#include "chunk_utils.hpp"
#include "ray_cast_voxel.hpp"

constexpr float SAFE_FRAC_PI_2 = M_PI_2 - 0.0001;

Player::Player(World& world): world(world) {}

void Player::onReady(mk::Game& game) {
	setPosition({ 0.f, CHUNK_SIZE + 1.f, 0.f });

	// Camera
	camera = addChild<mk::Camera3D>(game);
	camera->setPosition(getPosition());
	camera->setDirection({ 0.f, 0.f, 1.f });
	auto [win_w, win_h] = game.getRenderWindow().getSize().type<float>().vec_data;
	camera->setAspect(win_w / win_h);
	camera->setFov(75.f);

	player_ui = addChild<PlayerUI, 10'000>(game, *this);
}

void Player::onUpdate(mk::Game& game, const float dt) {
	if (game.getRenderWindow().getMouseCursorMode() == mk::Window::MouseMode::GRABBED) {
		// Rotate camera
		auto [mouse_dx, mouse_dy] = game.getMouseDelta().vec_data;
		auto [pitch, yaw, roll]   = camera->getPitchYawRoll().vec_data;
		yaw += mouse_dx * MOUSE_SENSITIVITY;
		pitch += -mouse_dy * MOUSE_SENSITIVITY;

		// Keep the camera's angle from going too high/low.
		if (pitch < -SAFE_FRAC_PI_2)
			pitch = -SAFE_FRAC_PI_2;
		else if (pitch > SAFE_FRAC_PI_2)
			pitch = SAFE_FRAC_PI_2;
		camera->setPitchYawRoll({ pitch, yaw, roll });

		// Select a block
		auto voxels = castVoxelRay(
			getPosition().type<double>(), camera->getDirection().type<double>(), 10.f
		);
		std::optional<mk::math::Vector3i> prev;
		for (auto voxel: voxels) {
			if (auto [chunk, chv] = world.getChunkAndPos(voxel); chunk) {
				if (chunk->getBlockType(chv.x, chv.y, chv.z) != VoxelType::AIR) {
					// Here mark a block
					// cube_outline.setPosition(voxel.type<float>());
					if (game.isMouseJustPressed(mk::input::MOUSE_LEFT)) {
						chunk->setBlock(chv.x, chv.y, chv.z, VoxelType::AIR, true);
					} else if (game.isMouseJustPressed(mk::input::MOUSE_RIGHT)) {
						if (prev) {
							if (auto [chunk_prev, chv_prev] = world.getChunkAndPos(*prev);
							    chunk_prev) {
								chunk_prev->setBlock(
									chv_prev.x, chv_prev.y, chv_prev.z, static_cast<VoxelType>(player_ui->getPlayerSlot() + 1), true
								);
							}
						}
					}
					break;
				}
			}
			prev = voxel;
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
}

void Player::onEvent(mk::Game& game, const mk::Event& event) {
	WorldEntity3D::onEvent(game, event);
	if (auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {}
}

mk::Camera3D* Player::getCamera() const { return camera; }
