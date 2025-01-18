#pragma once
#include "chunk_utils.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/TextureRect.hpp"

class Player;
class PlayerSlotBar;

enum class PlayerMode { FREE_MOUSE, PLAYING };

class PlayerUI final: public mk::WorldEntityUI {
public:
	explicit PlayerUI(Player& player);

	void onReady(mk::Game& game) override;

	void onEvent(mk::Game& game, const mk::Event& event) override;

	void setPlayerMode(PlayerMode mode, mk::Game& game);

	usize getPlayerSlot() const;

private:
	Player& player;
	PlayerMode player_mode;

	mk::gui::TextureRect* crosshair = nullptr;
	PlayerSlotBar* player_slot_bar = nullptr;
};
