#pragma once
#include "Chunk.hpp"
#include "Player.hpp"

#include "MKE/DrawContext.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"

#include "MKE/WorldEntity.hpp"

class World final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override;

	void onEvent(mk::Game& game, const mk::Event& event) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

	std::unordered_map<i32, std::unordered_map<i32, std::unordered_map<i32, Chunk*>>> chunks;

	Player* player{};
	bool    wireframe = false;
};
