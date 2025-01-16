#include "Chunk.hpp"
#include "Player.hpp"
#include "MKE/Nodes/3d/CubeShape.hpp"

#include <GLFW/glfw3.h>
#include <MKE/Game.hpp>

class World final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override {
		player            = addChild<Player>(game);
		// constexpr int CNT = 7;  // (CNT * 2 + 1) ** 2
		constexpr int CNT = 1;  // (CNT * 2 + 1) ** 2
		for (int x = -CNT; x <= CNT; x++) {
			for (int y = -CNT; y <= CNT; y++) {
				chunks.push_back(addChild<Chunk>(game, player->getCamera()));
				chunks.back()->setPosition(mk::math::Vector3f(x * CHUNK_SIZE, 0, y * CHUNK_SIZE));
			}
		}
		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
	}

	void onEvent(mk::Game& game, const mk::Event& event) override {
		if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
			if (ev->key == mk::input::KEY::T) {
				wireframe ^= 1;
				std::cout << "wireframe: " << wireframe << std::endl;
			}
			if (ev->key == mk::input::KEY::ESCAPE)
				game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::NORMAL);
		}
		if (const auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {
			if (ev->button == mk::input::MOUSE_LEFT
			    && game.getRenderWindow().getMouseCursorMode() == mk::Window::MouseMode::NORMAL) {
				game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
			}
		}
	}

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override {
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	std::list<Chunk*> chunks{};
	Player*           player{};
	bool              wireframe = false;
};

int main() {
	mk::Game game("settings.json");
	initTextures(game);
	game.addScene<World>();
	game.run();
}
