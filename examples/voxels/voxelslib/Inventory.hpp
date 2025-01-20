#pragma once
#include "chunk_utils.hpp"

#include "MKE/Ints.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/GUI/Label.hpp"
#include "MKE/Nodes/GUI/TextureRect.hpp"

#include <algorithm>
#include <array>

class HpBar;

class Inventory final: public mk::WorldEntityUI {
public:
	void onReady(mk::Game& game) override;

	void changeSlot(i32 shift);

	i32 getCurrentSlot() const;

	GameItem useHeldItem() const;
	GameItem getHeldItem() const;

	void addItems(GameItem item, i32 count);

private:
	void repositionSelection() const;

	static constexpr i32 SLOT_WIDTH   = 20;
	static constexpr i32 SLOT_HEIGHT  = 20;
	static constexpr i32 SLOT_PADDING = 2;
	static constexpr i32 SLOTS_W      = 9;
	static constexpr i32 SLOTS_H      = 4;
	static constexpr i32 STACK_SIZE   = 64;
	i32                  current_slot = 0;

	mk::gui::TextureRect* slot_bar      = nullptr;
	mk::gui::TextureRect* selected_slot = nullptr;

	class Slot final: public WorldEntityUI {
	public:
		void onReady(mk::Game& game) override;

		[[nodiscard]]
		GameItem getItem() const;

		void setItem(GameItem item);

		i32 getCount() const;

		void setCount(i32 count);

	private:
		GameItem item  = GameItem::AIR;
		i32      count = 0;

		mk::gui::TextureRect* texture{};
		mk::gui::Label*       label{};
	};

	std::array<Slot*, SLOTS_W * SLOTS_H> inventory_slots{};
};
