#pragma once
#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/TextureRect.hpp"

class World;

class Player final: public mk::WorldEntity3D {
public:
	explicit Player(World& world);
	void onReady(mk::Game& game) override;

	void onUpdate(mk::Game& game, float dt) override;
	void onEvent(mk::Game& game, const mk::Event& event) override;

	[[nodiscard]]
	mk::Camera3D* getCamera() const;

private:
	const float PLAYER_LERP_SPEED = 4.f;
	const float PLAYER_BOOST      = 5.f;
	const float PLAYER_SPEED      = 10.f;
	const float MOUSE_SENSITIVITY = 0.005f;

	mk::math::Vector3f move_vec;
	mk::Camera3D*      camera = nullptr;

	mk::gui::TextureRect* crosshair = nullptr;

	World& world;
};
