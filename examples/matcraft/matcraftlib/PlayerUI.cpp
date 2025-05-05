//
// Created by Mateusz Kolpa on 18/01/2025.
//

#include "PlayerUI.hpp"
#include "Player.hpp"
#include "Inventory.hpp"

class HpBar final: public mk::WorldEntityUI {
public:
	HpBar(usize max_hp = 20) {
		MK_ASSERT(max_hp % 2 == 0, "Hp must be even");
		hearts.resize(max_hp / 2);
	}

	void onReady(mk::Game& game) override {
		full_heart = game.resources().getTexture("full_heart.png");
		game.resources().setTextureSmooth("full_heart.png", false);
		half_heart = game.resources().getTexture("half_heart.png");
		game.resources().setTextureSmooth("half_heart.png", false);
		empty_heart = game.resources().getTexture("empty_heart.png");
		game.resources().setTextureSmooth("empty_heart.png", false);

		for (usize i = 0; i < hearts.size(); i++) {
			hearts[i] = addChild<mk::gui::TextureRect>(game);
			hearts[i]->setPosition(i * 18, 0);
			hearts[i]->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::BOTTOM);
			hearts[i]->setTexture(full_heart);
		}
	}

	void setHp(i32 hp) {
		MK_ASSERT(hp <= hearts.size() * 2, "Hp out of range");
		for (const auto& heart: hearts) {
			if (hp >= 2) {
				heart->setTexture(full_heart);
				hp -= 2;
			} else if (hp) {
				heart->setTexture(half_heart);
				hp--;
			} else {
				heart->setTexture(empty_heart);
			}
		}
	}

private:
	const mk::Texture* full_heart;
	const mk::Texture* half_heart;
	const mk::Texture* empty_heart;

	std::vector<mk::gui::TextureRect*> hearts;
};

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

	// Load player slot bar
	inv = addChild<Inventory>(game);
	inv->setScale({ 3.f });

	hp_bar = addChild<HpBar>(game, player.getMaxHp());
	repositionElements(game);
}

void PlayerUI::onEvent(mk::Game& game, const mk::Event& event) {
	if (auto ev = event.get<mk::Event::WindowResized>(); ev) repositionElements(game);
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

Inventory& PlayerUI::getInventory() const { return *inv; }

void PlayerUI::showHp() { hp_bar->show(); }

void PlayerUI::hideHp() { hp_bar->hide(); }

void PlayerUI::setHp(const i32 hp) { hp_bar->setHp(hp); }

void PlayerUI::repositionElements(const mk::Game& game) {
	auto [ww, wh] = game.getWindowSize().type<float>().vec_data;
	crosshair->setPosition(ww / 2.0f, wh / 2.f);
	inv->setPosition(ww / 2.f, wh);
	hp_bar->setPosition(inv->getPosition2D() - mk::math::Vector2f(20.0 * 4.5 * 3.0, 88.f));
}
