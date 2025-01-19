//
// Created by Mateusz Kolpa on 19/01/2025.
//

#include "Inventory.hpp"

void Inventory::onReady(mk::Game& game) {
	// Load slot bar
	auto slot_bar_texture = game.resources().getTexture("slot_bar.png");
	game.resources().setTextureSmooth("slot_bar.png", false);
	slot_bar                               = addChild<mk::gui::TextureRect>(game, slot_bar_texture);
	auto [slot_bar_width, slot_bar_height] = slot_bar_texture->getSize().type<float>().vec_data;
	slot_bar->setOrigin(slot_bar_width / 2.f, slot_bar_height + 5.f);
	const auto origin = slot_bar->getOrigin();

	auto selected_slot_texture = game.resources().getTexture("selected_slot.png");
	game.resources().setTextureSmooth("selected_slot.png", false);
	selected_slot = addChild<mk::gui::TextureRect, 1>(game, selected_slot_texture);
	selected_slot->setOrigin(origin);

	for (usize x = 0; x < SLOTS_W; ++x) {
		for (usize y = 0; y < 1; ++y) {
			auto slot                        = addChild<Slot, 2>(game);
			inventory_slots[y * SLOTS_W + x] = slot;
			slot->setPosition(
				x * SLOT_WIDTH + slot_bar->getBounds().left, slot_bar->getBounds().top
			);
		}
	}

	repositionSelection();
}

void Inventory::changeSlot(const i32 shift) {
	current_slot += shift;
	current_slot %= SLOTS_W;
	if (current_slot < 0) current_slot += SLOTS_W;
	repositionSelection();
}

i32 Inventory::getCurrentSlot() const { return current_slot; }

GameItem Inventory::useHeldItem() const {
	auto& slot = *inventory_slots[current_slot];
	if (const auto cnt = slot.getCount(); cnt > 0) {
		const auto item = slot.getItem();
		slot.setCount(cnt - 1);
		return item;
	}
	return GameItem::AIR;
}

GameItem Inventory::getHeldItem() const { return inventory_slots[current_slot]->getItem(); }

void Inventory::addItems(const GameItem item, i32 count) {
	MK_ASSERT(item != GameItem::AIR);
	for (auto& slot: inventory_slots) {
		if (slot->getItem() == GameItem::AIR) {
			slot->setItem(item);
			slot->setCount(count);
			count = 0;
		} else if (slot->getItem() == item) {
			const i32 to_add = std::min(count, STACK_SIZE - slot->getCount());
			slot->setCount(slot->getCount() + to_add);
			count -= to_add;
		}
		if (count == 0) break;
	}
}

void Inventory::repositionSelection() const {
	selected_slot->setPosition(current_slot * SLOT_WIDTH, 0);
}

void Inventory::Slot::onReady(mk::Game& game) {
	label = addChild<mk::gui::Label, 1>(game, game.getDefaultFont(), "0");
	label->setPosition({ SLOT_WIDTH - SLOT_PADDING, SLOT_WIDTH - SLOT_PADDING });
	label->setAlignment(mk::gui::HAlignment::RIGHT, mk::gui::VAlignment::BOTTOM);
	label->setTextSize(16);
	label->setScale(0.5f);
	texture = addChild<mk::gui::TextureRect>(game);
	texture->setScale(0.75f);
	texture->setPosition(SLOT_PADDING + 1, SLOT_PADDING + 1);
	texture->move(8.f * (1.f - texture->getScale().x), 8.f * (1.f - texture->getScale().y));

	hide();
}

GameItem Inventory::Slot::getItem() const { return item; }

void Inventory::Slot::setItem(const GameItem item) {
	this->item = item;
	texture->setTexture(getVoxelTexture(item, FaceDir::NORTH));
}

i32 Inventory::Slot::getCount() const { return count; }

void Inventory::Slot::setCount(const i32 count) {
	MK_ASSERT(count >= 0, "Count cannot be negative");
	this->count = count;
	if (count == 0) {
	} else {
		show();
		label->setString(std::to_string(count));
	}
}
