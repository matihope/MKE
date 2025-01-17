#include "World.hpp"

void World::onReady(mk::Game& game) {
	player = addChild<Player, 10>(game, *this);
	// constexpr int CNT = 3;  // (CNT * 2 + 1) ** 2
	constexpr int CNT = 0;  // (CNT * 2 + 1) ** 2
	for (int x = -CNT; x <= CNT; x++) {
		for (int y = -CNT; y <= CNT; y++) {
			chunks[x][y][0]
				= addChild<Chunk>(game, player->getCamera(), mk::math::Vector3i(x, 0, y));
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
