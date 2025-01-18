//
// Created by Mateusz Kolpa on 18/01/2025.
//

#include "PlayerUI.hpp"
#include "Player.hpp"

class PlayerSlotBar final: public mk::WorldEntityUI {
public:
	void onReady(mk::Game& game) override {
		// Load slot bar
		auto slot_bar_texture = game.resources().getTexture("slot_bar.png");
		game.resources().setTextureSmooth("slot_bar.png", false);
		slot_bar = addChild<mk::gui::TextureRect>(game, slot_bar_texture);
		auto [slot_bar_width, slot_bar_height] = slot_bar_texture->getSize().type<float>().vec_data;
		slot_bar->setOrigin(slot_bar_width / 2.f, slot_bar_height + 5.f);
		const auto origin = slot_bar->getOrigin();

		auto selected_slot_texture = game.resources().getTexture("selected_slot.png");
		game.resources().setTextureSmooth("selected_slot.png", false);
		selected_slot = addChild<mk::gui::TextureRect, 1>(game, selected_slot_texture);
		// selected_slot->setOrigin(origin + mk::math::Vector3f{1.f, 1.f, 0.f});
		selected_slot->setOrigin(origin);
		repositionSelection();
	}

	void changeSlot(const i32 shift) {
		current_slot += shift;
		current_slot %= slots;
		if (current_slot < 0) current_slot += slots;
		repositionSelection();
	}

	i32 getCurrentSlot() const { return current_slot; }

private:
	void repositionSelection() const {
		selected_slot->setPosition(
			(mk::math::Vector2i(1, 0) * current_slot * slot_size).type<float>()
		);
	}

	static constexpr i32 slot_size    = 20;
	static constexpr i32 slots        = 9;
	i32                  current_slot = 0;

	mk::gui::TextureRect* slot_bar      = nullptr;
	mk::gui::TextureRect* selected_slot = nullptr;
};

PlayerUI::PlayerUI(Player& player): player(player), player_mode(PlayerMode::FREE_MOUSE) {}

void PlayerUI::onReady(mk::Game& game) {
	setPlayerMode(PlayerMode::PLAYING, game);

	auto [win_w, win_h] = game.getWindowSize().type<float>().vec_data;

	// Load crosshair
	auto crosshair_texture = game.resources().getTexture("crosshair.png");
	game.resources().setTextureSmooth("crosshair.png", false);
	crosshair = addChild<mk::gui::TextureRect>(game, crosshair_texture);
	crosshair->setOrigin(crosshair_texture->getSize().type<float>() / 2.f);
	crosshair->setScale({ 2.f });
	crosshair->setPosition(win_w / 2.f, win_h / 2.f);

	// Load player slot bar
	player_slot_bar = addChild<PlayerSlotBar>(game);
	player_slot_bar->setPosition(win_w / 2.f, win_h);
	player_slot_bar->setScale({ 3.f });
}

void PlayerUI::onEvent(mk::Game& game, const mk::Event& event) {
	if (auto ev = event.get<mk::Event::WindowResized>(); ev)
		crosshair->setPosition(ev->new_size.type<float>() / 2.f);
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::ESCAPE) setPlayerMode(PlayerMode::FREE_MOUSE, game);
	}
	if (const auto ev = event.get<mk::Event::MouseButtonPressed>(); ev) {
		if (ev->button == mk::input::MOUSE_LEFT && player_mode == PlayerMode::FREE_MOUSE)
			setPlayerMode(PlayerMode::PLAYING, game);
	}
	if (const auto ev = event.get<mk::Event::MouseScrolled>(); ev)
		player_slot_bar->changeSlot(mk::math::sign(ev->delta.y));
}

void PlayerUI::setPlayerMode(const PlayerMode mode, mk::Game& game) {
	switch (mode) {
	case PlayerMode::FREE_MOUSE:
		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::NORMAL);
		break;
	case PlayerMode::PLAYING:
		game.getRenderWindow().setMouseCursorMode(mk::Window::MouseMode::GRABBED);
		break;
	}
	this->player_mode = mode;
}

usize PlayerUI::getPlayerSlot() const { return player_slot_bar->getCurrentSlot(); }
