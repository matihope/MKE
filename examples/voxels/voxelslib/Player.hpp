#pragma once

#include "PlayerUI.hpp"
#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"

class World;

enum class GameMode { SURVIVAL, CREATIVE };

class Player final: public mk::WorldEntity3D {
public:
	explicit Player(World& world, GameMode mode);
	void onReady(mk::Game& game) override;

	void initialReposition();

	void onUpdate(mk::Game& game, float dt) override;
	void onPhysicsUpdate(mk::Game& game, float dt) override;
	void onEvent(mk::Game& game, const mk::Event& event) override;

	[[nodiscard]]
	mk::Camera3D* getCamera() const;

	void     setGameMode(GameMode game_mode);
	GameMode getGameMode() const;

	[[nodiscard]]
	i32 getMaxHp() const;

	[[nodiscard]]
	i32 getPlayerHp() const;

	void setPlayerHp(mk::Game& game, i32 hp);

private:
	const float PLAYER_LERP_SPEED_FLYING = 4.f;
	const float PLAYER_BOOST_FLYING      = 5.f;
	const float PLAYER_SPEED_FLYING      = 10.f;
	const float PLAYER_SPEED_WALKING     = 5.f;
	const float PLAYER_BOOST_WALKING     = 1.5;
	const float PLAYER_JUMP_FORCE        = 6.8f;
	const float PLAYER_GRAVITY           = -0.32f;
	const float PLAYER_BUILD_RANGE_CREATIVE = 10.f;
	const float PLAYER_BUILD_RANGE_SURVIVAL = 7.f;

	const i32 PLAYER_MAX_HP = 20;
	i32       player_hp     = PLAYER_MAX_HP;

	const float PLAYER_LERP_SPEED_WALKING = 8.f;
	const float MOUSE_SENSITIVITY         = 0.001f;
	const float PLAYER_HEIGHT             = 1.8;
	const float PLAYER_WIDTH              = 0.5;
	const float PLAYER_EYE_OFFSET         = 0.15;  // (below player's top)

	void resolveUpdateSurvival(mk::Game& game, float dt);
	void resolveUpdateCreative(mk::Game& game, float dt);

	mk::math::Vector3f moveAndSlide(mk::Game& game, mk::math::Vector3f dspeed, float ddt);

	[[nodiscard]]
	std::array<mk::math::Vector3d, 2> getUpperLowerSphere() const;

	[[nodiscard]]
	bool playerIsNotInVoxel(mk::math::Vector3i voxel) const;

	mk::math::Vector3f move_vec;
	mk::Camera3D*      camera = nullptr;

	PlayerUI* player_ui{};
	GameMode  game_mode;
	World&    world;
};
