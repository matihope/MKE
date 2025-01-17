#pragma once
#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/TextureRect.hpp"

class Player final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override;

	void onUpdate(mk::Game& game, float dt) override;

	[[nodiscard]]
	mk::Camera3D* getCamera() const;

private:
	const float PLAYER_LERP_SPEED = 4.f;
	const float PLAYER_BOOST      = 5.f;
	const float PLAYER_SPEED      = 10.f;
	const float MOUSE_SENSITIVITY = 0.5f;

	mk::math::Vector3f move_vec;
	mk::Camera3D*      camera = nullptr;

	mk::gui::TextureRect* crosshair = nullptr;
};
