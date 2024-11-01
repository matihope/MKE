#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/3d/CubeShape.hpp"
#include "MKE/WorldEntity.hpp"

class Cube: public mk::CubeShape {
public:
	void onReady(mk::Game&) override { setOrigin(mk::math::Vector3f{ 0.5f }); }

	void onUpdate(mk::Game&, float dt) override { rotate(mk::math::Vector3f{ dt }); }
};

int main() {
	mk::Game game("settings.json");

	auto scene = game.addScene<mk::WorldEntity3D>();

	scene->addChild<Cube>(game);

	auto cam = scene->addChild<mk::Camera3D>(game);
	cam->setPosition({ 5.f });
	cam->lookAt({ 0.f });

	game.run();
}
