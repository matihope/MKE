#pragma once
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/Button.hpp"
#include "MKE/Nodes/GUI/Label.hpp"

class DeathScreen: public mk::WorldEntityUI {
public:
	void onReady(mk::Game& game) override;
	void onEvent(mk::Game& game, const mk::Event& event) override;
	void onUpdate(mk::Game& game, float dt) override;

private:
	void reposition(const mk::Game& game);
	mk::gui::Label*  label = nullptr;
	mk::gui::Button* menu_btn  = nullptr;
};
