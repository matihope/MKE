#include "MKE/Camera.hpp"
#include "Chunk.hpp"

#include <MKE/Game.hpp>

class World final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override {
		chunks.push_back(addChild<Chunk>(game));
		cam = addChild<mk::Camera3D>(game);
		cam->setPosition({ 100.f });
		cam->lookAt({ 0.f, 8.f, 0.f });
	}

	void onEvent(mk::Game& game, const mk::Event& event) override {
		if (auto ev = event.get<mk::Event::KeyPressed>(); ev) {
			if (ev->key == mk::input::KEY::T) {
				wireframe ^= 1;
				if (wireframe)
					glPolygonMode(GL_FRONT, GL_LINE);
				else
					glPolygonMode(GL_FRONT, GL_FILL);
			}
		}
	}

	void onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const override {

	}

	std::list<Chunk*> chunks{};
	mk::Camera3D*       cam;
	bool                wireframe = false;
};

int main() {
	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
