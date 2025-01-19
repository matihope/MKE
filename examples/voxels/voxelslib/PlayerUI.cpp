//
// Created by Mateusz Kolpa on 18/01/2025.
//

#include "PlayerUI.hpp"
#include "Player.hpp"
#include "Inventory.hpp"


PlayerUI::PlayerUI(Player& player): player(player), cursor_mode(CursorMode::FREE_MOUSE) {}

void PlayerUI::onFree(mk::Game& game) { setCursorMode(CursorMode::FREE_MOUSE, game); }

void PlayerUI::onReady(mk::Game& game) {
	setCursorMode(CursorMode::PLAYING, game);

	auto [win_w, win_h] = game.getWindowSize().type<float>().vec_data;

	// Load crosshair
	auto crosshair_texture = game.resources().getTexture("crosshair.png");
	game.resources().setTextureSmooth("crosshair.png", false);
	crosshair = addChild<mk::gui::TextureRect>(game, crosshair_texture);
	crosshair->setOrigin(crosshair_texture->getSize().type<float>() / 2.f);
	crosshair->setScale({ 2.f });
	crosshair->setPosition(win_w / 2.f, win_h / 2.f);

	// Load player slot bar
	inv = addChild<Inventory>(game);
	inv->setPosition(win_w / 2.f, win_h);
	inv->setScale({ 3.f });
}

void PlayerUI::onEvent(mk::Game& game, const mk::Event& event) {
	if (auto ev = event.get<mk::Event::WindowResized>(); ev) {
		auto [ww, wh] = ev->new_size.type<float>().vec_data;
		crosshair->setPosition(ww / 2.0f, wh / 2.f);
		inv->setPosition(ww / 2.f, wh);
	}
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::ESCAPE) setCursorMode(CursorMode::FREE_MOUSE, game);
		if (ev->key == mk::input::KEY::NUM_1) inv->changeSlot(0 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_2) inv->changeSlot(1 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_3) inv->changeSlot(2 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_4) inv->changeSlot(3 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_5) inv->changeSlot(4 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_6) inv->changeSlot(5 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_7) inv->changeSlot(6 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_8) inv->changeSlot(7 - inv->getCurrentSlot());
		if (ev->key == mk::input::KEY::NUM_9) inv->changeSlot(8 - inv->getCurrentSlot());
	}
	if (const auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {
		if (ev->button == mk::input::MOUSE_LEFT && cursor_mode == CursorMode::FREE_MOUSE)
			setCursorMode(CursorMode::PLAYING, game);
	}
	if (const auto ev = event.get<mk::Event::MouseScrolled>(); ev)
		inv->changeSlot(mk::math::sign(ev->delta.y));
}

void PlayerUI::setCursorMode(const CursorMode mode, mk::Game& game) {
	switch (mode) {
	case CursorMode::FREE_MOUSE: {
		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::NORMAL);
		game.getRenderWindow().setRawMouseMotion(false);
		break;
	}
	case CursorMode::PLAYING: {
		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
		game.getRenderWindow().setRawMouseMotion(true);
		break;
	}
	}
	this->cursor_mode = mode;
}

usize PlayerUI::getPlayerSlot() const { return inv->getCurrentSlot(); }

Inventory& PlayerUI::getInventory() const {
	return *inv;
}
