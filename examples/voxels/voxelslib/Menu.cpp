//
// Created by Mateusz Kolpa on 19/01/2025.
//

#include "Menu.hpp"

#include "World.hpp"
#include "MKE/Game.hpp"

void Menu::onReady(mk::Game& game) {
	game.setClearColor(mk::Color(34, 149, 32));
	title_bg = addChild<mk::gui::Label>(game, game.getDefaultFont(), "Matcraft");
	title_bg->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::TOP);
	title_bg->setOrigin({ 0, -40.f });
	title_bg->setTextSize(128.f);
	title_bg->setColor(mk::Color(21, 21, 21));
	title_fg = addChild<mk::gui::Label>(game, game.getDefaultFont(), "Matcraft");
	title_fg->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::TOP);
	title_fg->setOrigin({ 5.f, -45.f });
	title_fg->setTextSize(128.f);

	play = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Play");
	play->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::TOP);
	play->setMinSpaceBetween({ BUTTON_PADDING, BUTTON_PADDING });
	play->setTextSize(48.f);

	gamemode_btn = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Gamemode: Survival");
	gamemode_btn->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::TOP);
	gamemode_btn->setMinSpaceBetween({ BUTTON_PADDING, BUTTON_PADDING });
	gamemode_btn->setTextSize(48.f);

	world_size_lbl = addChild<mk::gui::Label>(game, game.getDefaultFont());
	world_size_lbl->setTextSize(48.f);
	world_size_lbl->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);
	world_size_up = addChild<mk::gui::Button>(game, game.getDefaultFont(), "+");
	world_size_up->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::CENTER);
	world_size_up->setMinSpaceBetween({ BUTTON_PADDING, BUTTON_PADDING });
	world_size_down = addChild<mk::gui::Button>(game, game.getDefaultFont(), "-");
	world_size_down->setAlignment(mk::gui::HAlignment::RIGHT, mk::gui::VAlignment::CENTER);
	world_size_down->setMinSpaceBetween({ BUTTON_PADDING, BUTTON_PADDING });

	setLabels(game);
}

void Menu::onReReady(mk::Game& game) {
	game.setClearColor(mk::Color(34, 149, 32));
	setLabels(game);
}

void Menu::onEvent(mk::Game& game, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::WindowResized>(); ev) reLayout(game);
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::M) game.addScene<World>(GameMode::SURVIVAL, 1);
	}
}

void Menu::onUpdate(mk::Game& game, float dt) {
	if (play->isPressed()) game.addScene<World>(gamemode, world_size);

	if (world_size_up->isPressed() && world_size < 20) world_size++;
	if (world_size_down->isPressed() && world_size > 0) world_size--;
	if (gamemode_btn->isPressed()) gamemode = static_cast<GameMode>(static_cast<int>(gamemode) ^ 1);

	if (world_size_down->isPressed() || world_size_up->isPressed() || gamemode_btn->isPressed())
		setLabels(game);
}

void Menu::setLabels(const mk::Game& game) const {
	world_size_lbl->setString("World size: " + std::to_string(world_size));
	gamemode_btn->setText(
		std::string("Gamemode: ") + (gamemode == GameMode::SURVIVAL ? "Survival" : "Creative")
	);
	reLayout(game);
}

void Menu::reLayout(const mk::Game& game) const {
	const auto [win_w, win_h] = game.getWindowSize().vec_data;
	const auto [mid_w, mid_h] = (game.getWindowSize() / 2.f).vec_data;
	title_bg->setPosition(mid_w, 0.f);
	title_fg->setPosition(mid_w, 0.f);

	play->setPosition(mid_w, static_cast<float>(mid_h));
	gamemode_btn->setPosition(mid_w, play->getBounds().bottom() + BUTTON_PADDING);
	gamemode_btn->setMinSize({ win_w / 3.5f, 0.f });
	play->setMinSize({ gamemode_btn->getBounds().width, 0.f });

	world_size_lbl->setPosition(win_w / 2.f, win_h - win_h / 4.f);
	const auto lbl_bounds = world_size_lbl->getBounds();
	world_size_down->setPosition(
		lbl_bounds.left - BUTTON_PADDING, lbl_bounds.top + lbl_bounds.height / 2.f
	);
	world_size_up->setPosition(
		lbl_bounds.right() + BUTTON_PADDING, lbl_bounds.top + lbl_bounds.height / 2.f
	);
}
