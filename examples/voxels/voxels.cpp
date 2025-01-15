#include "MKE/Camera.hpp"
#include "Chunk.hpp"
#include "MKE/Nodes/3d/CubeShape.hpp"

#include <MKE/Game.hpp>

class World final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override {
		cam = addChild<mk::Camera3D>(game);
		cam->setPosition({ 24, 64.f, 64.f });
		cam->lookAt({ 16.f, 32.f, 16.f });
		chunks.push_back(addChild<Chunk>(game));
	}

	void onEvent(mk::Game& game, const mk::Event& event) override {
		if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
			if (ev->key == mk::input::KEY::T) {
				wireframe ^= 1;
				std::cout << "wireframe: " << wireframe << std::endl;
			}
		}
	}

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override {
		if (wireframe)
			glPolygonMode(GL_FRONT, GL_LINES);
		else
			glPolygonMode(GL_FRONT, GL_FILL);
	}

	std::list<Chunk*> chunks{};
	mk::Camera3D*     cam;
	bool              wireframe = false;
};

int main() {
	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
