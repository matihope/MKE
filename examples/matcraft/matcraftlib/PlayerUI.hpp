#pragma once
#include "chunk_utils.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/TextureRect.hpp"

class Player;
class Inventory;
class HpBar;

enum class CursorMode { FREE_MOUSE, PLAYING };

class PlayerUI final: public mk::WorldEntityUI {
public:
	explicit PlayerUI(Player& player);

	void onFree(mk::Game& game) override;

	void onReady(mk::Game& game) override;

	void onEvent(mk::Game& game, const mk::Event& event) override;

	void setCursorMode(CursorMode mode, mk::Game& game);

	usize getPlayerSlot() const;

	[[nodiscard]]
	Inventory& getInventory() const;

	void showHp();
	void hideHp();
	void setHp(i32 hp);


private:
	void repositionElements(const mk::Game& game);
	Player&    player;
	CursorMode cursor_mode;

	mk::gui::TextureRect* crosshair = nullptr;
	Inventory*            inv       = nullptr;
	HpBar*                hp_bar    = nullptr;
};
