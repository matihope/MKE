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
		auto eye_position = camera->getPosition();
		auto voxels       = castVoxelRay(
            eye_position.type<double>(), camera->getDirection().type<double>(), 10.f
        );
		std::optional<mk::math::Vector3i> prev;
		world.chunk_shader.setBool("highlight.on", false);
		world.chunk_shader.setVector3f("player_position", eye_position);
		for (auto voxel: voxels) {
			if (auto [chunk, chv] = world.getChunkAndPos(voxel); chunk) {
				if (chunk->getBlockType(chv.x, chv.y, chv.z) != VoxelType::AIR) {
					// Here mark a block
					world.chunk_shader.setBool("highlight.on", true);
					world.chunk_shader.setVector3f("highlight.position", voxel.type<float>());
					if (game.isMouseJustPressed(mk::input::MOUSE_LEFT)) {
						chunk->setBlock(chv.x, chv.y, chv.z, VoxelType::AIR, true);
					} else if (game.isMouseJustPressed(mk::input::MOUSE_RIGHT)) {
						if (prev) {
							if (auto [chunk_prev, chv_prev] = world.getChunkAndPos(*prev);
							    chunk_prev) {
								chunk_prev->setBlock(
									chv_prev.x,
									chv_prev.y,
									chv_prev.z,
									static_cast<VoxelType>(player_ui->getPlayerSlot() + 1),
									true
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

		const auto move_dir = forward * player_input.z + right * player_input.x;
		move_vec            = move_vec.lerp(move_dir, dt * PLAYER_LERP_SPEED, 1e-4f);
		move_vec            = moveAndSlide(move_vec + mk::math::Vector3fUP * player_input.y, dt);
		move_vec.y          = 0;

		camera->setPosition(
			getPosition() + mk::math::Vector3f(0.f, PLAYER_HEIGHT - PLAYER_EYE_OFFSET, 0.f)
		);
	}
}

void Player::onEvent(mk::Game& game, const mk::Event& event) {
	WorldEntity3D::onEvent(game, event);
	if (auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {}
}

mk::Camera3D* Player::getCamera() const { return camera; }

constexpr mk::math::Vector3f getVolumeOverlap(
	const mk::math::Vector3f pos1,
	const mk::math::Vector3f size1,
	const mk::math::Vector3f pos2,
	const mk::math::Vector3f size2
) {
	mk::math::Vector3f result;
	for (usize i = 0; i < 3; i++) {
		if (pos1.vec_data[i] <= pos2.vec_data[i]
		    && pos2.vec_data[i] <= pos1.vec_data[i] + size1.vec_data[i]) {
			result.vec_data[i] = pos1.vec_data[i] + size1.vec_data[i] - pos2.vec_data[i];
			result.vec_data[i] -= std::max(
				0.f, pos1.vec_data[i] + size1.vec_data[i] - pos2.vec_data[i] - size2.vec_data[i]
			);
		} else if (pos1.vec_data[i] <= pos2.vec_data[i] + size2.vec_data[i]
		           && pos2.vec_data[i] + size2.vec_data[i]
		                  <= pos1.vec_data[i] + size1.vec_data[i]) {
			result.vec_data[i] = pos2.vec_data[i] + size2.vec_data[i] - pos1.vec_data[i];
			result.vec_data[i] -= std::max(
				0.f, pos2.vec_data[i] + size2.vec_data[i] - pos1.vec_data[i] - size1.vec_data[i]
			);
		}
	}
	return result;
}

mk::math::Vector3f Player::moveAndSlide(mk::math::Vector3f speed, const float dt) {
	speed *= dt;
	if (!mk::math::isZero(speed.lengthSquared())) {
		const auto pos = getGlobalPosition();

		const auto upper_sphere
			= pos + mk::math::Vector3f(0, PLAYER_HEIGHT - PLAYER_WIDTH / 2.f, 0);
		const auto lower_sphere = pos + mk::math::Vector3f(0, PLAYER_WIDTH / 2.f, 0);

		for (auto sph: { upper_sphere, lower_sphere }) {
			// upper sphere
			for (i32 dx = -1; dx < 2; dx++) {
				for (i32 dy = -1; dy < 2; dy++) {
					for (i32 dz = -1; dz < 2; dz++) {
						for (usize coord = 0; coord < 3; coord++) {
							auto sph_now = sph
							             + mk::math::Vector3f(
											   coord == 0 ? speed.vec_data[0] : 0.f,
											   coord == 1 ? speed.vec_data[1] : 0.f,
											   coord == 2 ? speed.vec_data[2] : 0.f
										 );
							auto sph_voxel = mk::math::Vector3i(
								std::floorf(sph_now.x),
								std::floorf(sph_now.y),
								std::floorf(sph_now.z)
							);

							auto tested_voxel = sph_voxel + mk::math::Vector3i(dx, dy, dz);
							auto [chunk, vox] = world.getChunkAndPos(tested_voxel);
							if (chunk
							    && chunk->getBlockType(vox.x, vox.y, vox.z) != VoxelType::AIR) {
								auto overlap = getVolumeOverlap(
									tested_voxel.type<float>(),
									{ 1.f },
									sph_now - mk::math::Vector3f(PLAYER_WIDTH / 2.f),
									{ PLAYER_WIDTH }
								);
								if (overlap.x && overlap.y && overlap.z) {
									// speed.vec_data[coord]
									// 	-= std::max(
									// 		   0.f,
									// 		   abs(speed.vec_data[coord]
									//                - overlap.vec_data[coord]
									//                      * mk::math::sign(speed.vec_data[coord]))
									// 	   )
									//      * mk::math::sign(speed.vec_data[coord]);
									speed.vec_data[coord] = 0;
								}
							}
						}
					}
				}
			}
		}
	}
	move(speed);
	return speed / dt;
}
