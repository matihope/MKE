#pragma once
#include "Player.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/Button.hpp"
#include "MKE/Nodes/GUI/Label.hpp"

class Menu final: public mk::WorldEntityUI {
public:
	void onReady(mk::Game& game) override;
	void onReReady(mk::Game& game) override;
	void onEvent(mk::Game& game, const mk::Event& event) override;

	void onUpdate(mk::Game& game, float dt) override;

private:
	void             setLabels(const mk::Game& game) const;
	void             reLayout(const mk::Game& game) const;
	mk::gui::Label*  title_bg     = nullptr;
	mk::gui::Label*  title_fg     = nullptr;
	mk::gui::Button* play         = nullptr;
	mk::gui::Button* gamemode_btn = nullptr;
	PlayerMode       gamemode     = PlayerMode::SURVIVAL;

	mk::gui::Label*        world_size_lbl  = nullptr;
	mk::gui::Button*       world_size_up   = nullptr;
	mk::gui::Button*       world_size_down = nullptr;
	int                    world_size      = 10;
	static constexpr float BUTTON_PADDING  = 10;
};
