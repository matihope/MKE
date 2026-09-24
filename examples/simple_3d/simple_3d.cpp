#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/3d/CubeShape.hpp"

constexpr float SAFE_FRAC_PI_2    = M_PI_2 - 0.0001;
constexpr float MOUSE_SENSITIVITY = 0.001f;

class FPVCamera: public mk::Camera3D {
public:
	void onReady(mk::Game& game) override {
		setPosition(mk::math::Vector3f{ 5.f });
		lookAt({ 0.f });
		auto [win_w, win_h] = game.getRenderWindow().getSize().type<float>().vec_data;
		setAspect(win_w / win_h);
		setFov(90.f * std::sqrt(win_w / win_h));

		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
		game.getRenderWindow().setRawMouseMotion(true);
	}

	void onUpdate(mk::Game& game, float dt) override {
		mk::math::Vector3f input;
		if (game.isKeyPressed(mk::input::KEY::W)) input.z = 1.f;
		if (game.isKeyPressed(mk::input::KEY::S)) input.z = -1.f;
		if (game.isKeyPressed(mk::input::KEY::A)) input.x = -1.f;
		if (game.isKeyPressed(mk::input::KEY::D)) input.x = 1.f;

		// Rotate camera
		auto [mouse_dx, mouse_dy] = game.getMouseDelta().vec_data;
		auto [pitch, yaw, roll]   = getPitchYawRoll().vec_data;
		yaw += mouse_dx * MOUSE_SENSITIVITY;
		pitch += -mouse_dy * MOUSE_SENSITIVITY;

		// Keep the camera's angle from going too high/low.
		if (pitch < -SAFE_FRAC_PI_2)
			pitch = -SAFE_FRAC_PI_2;
		else if (pitch > SAFE_FRAC_PI_2)
			pitch = SAFE_FRAC_PI_2;
		setPitchYawRoll({ pitch, yaw, roll });

		auto forward = getDirection();
		auto right   = -cross(mk::math::Vector3fUP, getDirection()).normalizeOrZero();

		const auto move_dir = forward * input.z + right * input.x;

		move(move_dir.normalizeOrZero() * dt * 5.f);
	}
};

class World: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override {
		camera = addChild<FPVCamera>(game);

		cube = addChild<mk::CubeShape>(game);
		cube->setScale({ 1.f, 1.f, 1.f });
		cube->setPosition({ 0, 0, 0 });
	}

private:
	mk::CubeShape* cube;
	FPVCamera*     camera;
};

int main() {
	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
