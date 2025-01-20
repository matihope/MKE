//
// Created by Mateusz Kolpa on 20/01/2025.
//

#include "DeathScreen.hpp"

#include "MKE/Game.hpp"

void DeathScreen::onReady(mk::Game& game) {
	game.setClearColor(mk::Color(0.8, 0.2, 0.2));
	label = addChild<mk::gui::Label>(game, game.getDefaultFont(), "You have died");
	label->setTextSize(128.f);
	label->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);

	menu_btn = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Back to menu");
	menu_btn->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);
	menu_btn->setMinSpaceBetween({ 10.f });

	reposition(game);
}

void DeathScreen::onEvent(mk::Game& game, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::WindowResized>(); ev) reposition(game);
}

void DeathScreen::onUpdate(mk::Game& game, float dt) {
	if (menu_btn->isPressed()) game.popScene();
}

void DeathScreen::reposition(const mk::Game& game) {
	auto [win_w, win_h] = game.getWindowSize().vec_data;
	menu_btn->setPosition(win_w / 2, win_h / 2);
	menu_btn->setMinSize({ win_w / 4.f, 0.f });
	label->setPosition(win_w / 2, win_h / 4.f);
}
