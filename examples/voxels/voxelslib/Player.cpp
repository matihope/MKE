#include "Player.hpp"

#include "DeathScreen.hpp"
#include "Inventory.hpp"
#include "World.hpp"
#include "chunk_utils.hpp"
#include "ray_cast_voxel.hpp"

constexpr float SAFE_FRAC_PI_2 = M_PI_2 - 0.0001;

Player::Player(World& world, const GameMode mode): game_mode(mode), world(world) {}

void Player::onReady(mk::Game& game) {
	// Camera
	camera = addChild<mk::Camera3D>(game);
	camera->setDirection({ 0.f, 0.f, 1.f });
	auto [win_w, win_h] = game.getRenderWindow().getSize().type<float>().vec_data;
	camera->setAspect(win_w / win_h);
	recalcFov(game);

	player_ui = addChild<PlayerUI, 10'000>(game, *this);
	if (game_mode == GameMode::CREATIVE)
		for (usize id = 1; id < VOXEL_TYPES; ++id)
			player_ui->getInventory().addItems(static_cast<GameItem>(id), 64);

	setGameMode(game_mode);
}

void Player::initialReposition() {
	i32 pos = 256;
	while (pos) {
		auto [ch, vox] = world.getChunkAndPos({ 0, pos - 1, 0 });
		if (!ch || ch->getBlockType(vox.type<u32>()) == GameItem::AIR)
			pos--;
		else
			break;
	}
	setPosition({ 0.5f, static_cast<float>(pos) + 0.1f, 0.5f });
	camera->setPosition(getPosition());
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
            eye_position.type<double>(),
            camera->getDirection().type<double>(),
            game_mode == GameMode::CREATIVE ? PLAYER_BUILD_RANGE_CREATIVE
												  : PLAYER_BUILD_RANGE_SURVIVAL
        );
		std::optional<mk::math::Vector3i> prev;
		world.chunk_shader.setBool("highlight.on", false);
		world.chunk_shader.setVector3f("player_position", eye_position);
		for (auto voxel: voxels) {
			if (auto [chunk, chv] = world.getChunkAndPos(voxel); chunk) {
				if (chunk->getBlockType(chv.x, chv.y, chv.z) != GameItem::AIR) {
					// Mark a block
					world.chunk_shader.setBool("highlight.on", true);
					world.chunk_shader.setVector3i("highlight.position", voxel);
					if (game.isMouseJustPressed(mk::input::MOUSE_LEFT)) {
						const auto block_type = chunk->getBlockType(chv.x, chv.y, chv.z);
						if (!(game_mode == GameMode::SURVIVAL && block_type == GameItem::BEDROCK)) {
							if (game_mode == GameMode::SURVIVAL)
								player_ui->getInventory().addItems(
									chunk->getBlockType(chv.x, chv.y, chv.z), 1
								);
							chunk->setBlock(chv.x, chv.y, chv.z, GameItem::AIR, true);
						}
					} else if (game.isMouseJustPressed(mk::input::MOUSE_RIGHT)) {
						if (prev && playerIsNotInVoxel(*prev)) {
							if (auto [chunk_prev, chv_prev] = world.getChunkAndPos(*prev);
							    chunk_prev) {
								const auto type = game_mode == GameMode::CREATIVE
								                    ? player_ui->getInventory().getHeldItem()
								                    : player_ui->getInventory().useHeldItem();
								if (type != GameItem::AIR) {
									chunk_prev->setBlock(
										chv_prev.x, chv_prev.y, chv_prev.z, type, true
									);
								}
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

void Player::onPhysicsUpdate(mk::Game& game, const float dt) {
	if (dt <= 2.f / 60.f) {
		// Move the player
		switch (game_mode) {
		case GameMode::SURVIVAL:
			resolveUpdateSurvival(game, dt);
			break;
		case GameMode::CREATIVE:
			resolveUpdateCreative(game, dt);
			break;
		}

		camera->setPosition(
			getPosition() + mk::math::Vector3f(0.f, PLAYER_HEIGHT - PLAYER_EYE_OFFSET, 0.f)
		);
	}
}

void Player::onEvent(mk::Game& game, const mk::Event& event) {
	WorldEntity3D::onEvent(game, event);
	if (auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::G && game.isKeyPressed(mk::input::KEY::LEFT_CONTROL))
			setGameMode(game_mode == GameMode::SURVIVAL ? GameMode::CREATIVE : GameMode::SURVIVAL);
		if (ev->key == mk::input::KEY::MINUS) setPlayerHp(game, getPlayerHp() - 1);
		if (ev->key == mk::input::KEY::EQUAL) setPlayerHp(game, getPlayerHp() + 1);
		if (ev->key == mk::input::KEY::O) world.setFogDistance(world.getFogDistance() + 1);
		if (ev->key == mk::input::KEY::L && world.getFogDistance() > 1)
			world.setFogDistance(world.getFogDistance() - 1);
	}
	if (event.is<mk::Event::WindowResized>()) recalcFov(game);
}

mk::Camera3D* Player::getCamera() const { return camera; }

void Player::setGameMode(const GameMode game_mode) {
	this->game_mode = game_mode;
	if (game_mode == GameMode::CREATIVE)
		player_ui->hideHp();
	else
		player_ui->showHp();
}

GameMode Player::getGameMode() const { return game_mode; }

i32 Player::getMaxHp() const { return PLAYER_MAX_HP; }

i32 Player::getPlayerHp() const { return player_hp; }

void Player::setPlayerHp(mk::Game& game, i32 hp) {
	hp = std::min(hp, PLAYER_MAX_HP);
	hp = std::max(hp, 0);
	if (hp <= 0)
		game.replaceTopScene<DeathScreen>();
	else {
		player_hp = hp;
		player_ui->setHp(hp);
	}
}

template<class K>
requires std::is_arithmetic_v<K> constexpr mk::math::Vector3<K> getVolumeOverlap(
	const mk::math::Vector3<K> pos1,
	const mk::math::Vector3<K> size1,
	const mk::math::Vector3<K> pos2,
	const mk::math::Vector3<K> size2
) {
	mk::math::Vector3<K> result;
	for (usize i = 0; i < 3; i++) {
		if (pos1.vec_data[i] <= pos2.vec_data[i]
		    && pos2.vec_data[i] <= pos1.vec_data[i] + size1.vec_data[i]) {
			result.vec_data[i] = pos1.vec_data[i] + size1.vec_data[i] - pos2.vec_data[i];
			result.vec_data[i] -= std::max<K>(
				K(0), pos1.vec_data[i] + size1.vec_data[i] - pos2.vec_data[i] - size2.vec_data[i]
			);
		} else if (pos1.vec_data[i] <= pos2.vec_data[i] + size2.vec_data[i]
		           && pos2.vec_data[i] + size2.vec_data[i]
		                  <= pos1.vec_data[i] + size1.vec_data[i]) {
			result.vec_data[i] = pos2.vec_data[i] + size2.vec_data[i] - pos1.vec_data[i];
			result.vec_data[i] -= std::max<K>(
				K(0), pos2.vec_data[i] + size2.vec_data[i] - pos1.vec_data[i] - size1.vec_data[i]
			);
		}
	}
	if (result.x && result.y && result.z) return result;
	return { K(0) };
}

void Player::recalcFov(const mk::Game& game) {
	auto [win_w, win_h] = game.getWindowSize().type<float>().vec_data;
	camera->setFov(90.f * std::sqrt(win_w / win_h));
}

void Player::resolveUpdateSurvival(mk::Game& game, const float dt) {
	using namespace mk::input;

	mk::math::Vector3f player_input{ 0.f };
	player_input.z = game.isKeyPressed(KEY::W) - game.isKeyPressed(KEY::S);
	player_input.x = game.isKeyPressed(KEY::D) - game.isKeyPressed(KEY::A);

	player_input = player_input.normalizeOrZero() * PLAYER_SPEED_WALKING;
	if (game.isKeyPressed(KEY::LEFT_CONTROL)) player_input *= PLAYER_BOOST_WALKING;

	if (game.isKeyPressed(KEY::SPACE) && move_vec.y == 0) move_vec.y = PLAYER_JUMP_FORCE;

	auto forward = camera->getDirection();
	forward.y    = 0;
	forward      = forward.normalizeOrZero();
	auto right   = -cross(mk::math::Vector3fUP, forward).normalizeOrZero();

	const auto move_dir
		= forward * player_input.z + right * player_input.x + mk::math::Vector3fUP * move_vec.y;
	move_vec = move_vec.lerp(move_dir, dt * PLAYER_LERP_SPEED_WALKING, 1e-4f);
	move_vec.y += PLAYER_GRAVITY;
	move_vec = moveAndSlide(game, move_vec, dt);
}

void Player::resolveUpdateCreative(mk::Game& game, const float dt) {
	using namespace mk::input;

	mk::math::Vector3f player_input;
	player_input.z = game.isKeyPressed(KEY::W) - game.isKeyPressed(KEY::S);
	player_input.x = game.isKeyPressed(KEY::D) - game.isKeyPressed(KEY::A);
	player_input.y = game.isKeyPressed(KEY::SPACE) - game.isKeyPressed(KEY::LEFT_SHIFT);
	player_input   = player_input.normalizeOrZero() * PLAYER_SPEED_FLYING;
	if (game.isKeyPressed(KEY::LEFT_CONTROL)) player_input *= PLAYER_BOOST_FLYING;

	auto forward     = camera->getDirection();
	forward.y        = 0;
	forward          = forward.normalizeOrZero();
	const auto right = -cross(mk::math::Vector3fUP, forward).normalizeOrZero();

	const auto move_dir = forward * player_input.z + right * player_input.x;
	move_vec            = move_vec.lerp(move_dir, dt * PLAYER_LERP_SPEED_FLYING, 1e-4f);
	move_vec            = moveAndSlide(game, move_vec + mk::math::Vector3fUP * player_input.y, dt);
	move_vec.y          = 0;
}

mk::math::Vector3f
	Player::moveAndSlide(mk::Game& game, const mk::math::Vector3f speed, const float dt) {
	auto       dspeed         = speed.type<double>();
	auto       ddt            = static_cast<double>(dt);
	const auto original_speed = dspeed;
	dspeed *= ddt;
	if (!mk::math::isZero(dspeed.lengthSquared())) {
		for (auto sph: getUpperLowerSphere()) {
			for (i32 dx = -1; dx < 2; dx++) {
				for (i32 dy = -1; dy < 2; dy++) {
					for (i32 dz = -1; dz < 2; dz++) {
						for (usize coord = 0; coord < 3; coord++) {
							auto sph_now = sph
							             + mk::math::Vector3d(
											   coord == 0 ? dspeed.vec_data[0]: 0.f,
											   coord == 1 ? dspeed.vec_data[1] : 0.f,
											   coord == 2 ? dspeed.vec_data[2] : 0.f
										 );
							auto sph_voxel = mk::math::Vector3i(
								std::floorf(sph_now.x),
								std::floorf(sph_now.y),
								std::floorf(sph_now.z)
							);

							auto tested_voxel = sph_voxel + mk::math::Vector3i(dx, dy, dz);
							auto [chunk, vox] = world.getChunkAndPos(tested_voxel);
							if (chunk
							    && chunk->getBlockType(vox.x, vox.y, vox.z) != GameItem::AIR) {
								auto overlap = getVolumeOverlap(
									tested_voxel.type<double>(),
									{ 1. },
									sph_now - mk::math::Vector3d(PLAYER_WIDTH / 2.),
									{ PLAYER_WIDTH }
								);
								if (overlap.x && overlap.y && overlap.z) {
									// Resolve hit
									if (game_mode == GameMode::SURVIVAL && coord == 1
									    && speed.y < 0.f) {
										const auto dmg = std::pow(-(speed.y / 10.f), 2.5);
										setPlayerHp(
											game, static_cast<i32>(std::ceil(getPlayerHp() - dmg))
										);
									}
									dspeed.vec_data[coord] = 0.;
								}
							}
						}
					}
				}
			}
		}
	}
	move(dspeed.type<float>());
	return { dspeed.x == original_speed.x * ddt ? speed.x : 0.f,
		     dspeed.y == original_speed.y * ddt ? speed.y : 0.f,
		     dspeed.z == original_speed.z * ddt ? speed.z : 0.f };
}

std::array<mk::math::Vector3d, 2> Player::getUpperLowerSphere() const {
	const auto pos          = getGlobalPosition().type<double>();
	const auto upper_sphere = pos + mk::math::Vector3d(0, PLAYER_HEIGHT - PLAYER_WIDTH / 2., 0);
	const auto lower_sphere = pos + mk::math::Vector3d(0, PLAYER_WIDTH / 2., 0);
	return { upper_sphere, lower_sphere };
}

bool Player::playerIsNotInVoxel(const mk::math::Vector3i voxel) const {
	for (const auto sph: getUpperLowerSphere()) {
		const auto overlap = getVolumeOverlap(
			voxel.type<double>(),
			{ 1.0 },
			sph - mk::math::Vector3d{ PLAYER_WIDTH / 2. },
			{ PLAYER_WIDTH }
		);
		if (overlap.x && overlap.y && overlap.z) return false;
	}
	return true;
}
