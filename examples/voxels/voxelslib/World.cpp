#include "World.hpp"

void World::onReady(mk::Game& game) {
	player = addChild<Player, 10>(game, *this);
	constexpr int CNT = 7;  // (CNT * 2 + 1) ** 2 * 2
	for (int x = -CNT; x <= CNT; x++) {
		for (int y = 0; y <= 2; y++) {
			for (int z = -CNT; z <= CNT; z++) {
				chunks[x][y][z]
					= addChild<Chunk>(game, player->getCamera(), mk::math::Vector3i(x, y, z));
			}
		}
	}
	game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
}

void World::onEvent(mk::Game& game, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::T) wireframe ^= 1;
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

void World::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const {
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
