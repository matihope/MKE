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

	// Load crosshair
	auto crosshair_texture = game.resources().getTexture("crosshair.png");
	game.resources().setTextureSmooth("crosshair.png", false);
	crosshair = addChild<mk::gui::TextureRect, 1>(game, crosshair_texture);
	crosshair->setPosition(getPosition().x, getPosition().y, 1.f);
	crosshair->setOrigin(crosshair_texture->getSize().type<float>() / 2.f);
	crosshair->setScale({ 2.f });
	crosshair->setPosition(game.getRenderWindow().getSize().type<float>() / 2.f);
}

void Player::onUpdate(mk::Game& game, const float dt) {
	// Rotate camera
	if (game.getRenderWindow().getMouseCursorMode() == mk::Window::MouseMode::GRABBED) {
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

constexpr std::pair<mk::math::Vector3i, mk::math::Vector3i>
	getChunkAndPos(const mk::math::Vector3i world_pos) {
	auto [x, y, z] = (world_pos % CHUNK_SIZE).vec_data;
	if (x < 0) x += CHUNK_SIZE;
	if (y < 0) y += CHUNK_SIZE;
	if (z < 0) z += CHUNK_SIZE;
	auto ch_x = mk::math::customDiv(world_pos.x, CHUNK_SIZE);
	auto ch_y = mk::math::customDiv(world_pos.y, CHUNK_SIZE);
	auto ch_z = mk::math::customDiv(world_pos.z, CHUNK_SIZE);
	return { { ch_x, ch_y, ch_z }, { x, y, z } };
}

void Player::onEvent(mk::Game& game, const mk::Event& event) {
	WorldEntity3D::onEvent(game, event);
	if (auto ev = event.get<mk::Event::WindowResized>(); ev)
		crosshair->setPosition(ev->new_size.type<float>() / 2.f);
	if (auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {
		auto voxels = voxels::castVoxelRay(
			getPosition().type<double>(), camera->getDirection().type<double>(), 10.f
		);
		std::optional<mk::math::Vector3i> prev;
		for (auto voxel: voxels) {
			auto [ch, vox] = getChunkAndPos(voxel);
			if (const auto chunk = world.chunks[ch.x][ch.y][ch.z]; chunk) {
				if (chunk->getBlockType(vox.x, vox.y, vox.z) != VoxelType::AIR) {
					if (ev->button == mk::input::MOUSE_LEFT) {
						chunk->setBlock(vox.x, vox.y, vox.z, VoxelType::AIR, true);
					} else if (ev->button == mk::input::MOUSE_RIGHT) {
						if (prev) {
							auto [ch_prev, vox_prev] = getChunkAndPos(*prev);
							if (const auto chunk_prev = world.chunks[ch_prev.x][ch_prev.y][ch_prev.z]; chunk_prev) {
								chunk_prev->setBlock(vox_prev.x, vox_prev.y, vox_prev.z, VoxelType::STONE, true);
							}
						}
					}
					break;
				}
			}
			prev = voxel;
		}
	}
}

mk::Camera3D* Player::getCamera() const { return camera; }
