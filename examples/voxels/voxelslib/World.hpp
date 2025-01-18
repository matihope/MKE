#pragma once
#include "Chunk.hpp"
#include "Player.hpp"

#include "MKE/DrawContext.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"

#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"

class World final: public mk::WorldEntity3D {
public:
	void onReady(mk::Game& game) override;

	void onEvent(mk::Game& game, const mk::Event& event) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

	// TODO: Make those two private
	std::list<Chunk>                                                                  chunk_list;
	std::unordered_map<i32, std::unordered_map<i32, std::unordered_map<i32, Chunk*>>> chunks;

	std::pair<Chunk*, mk::math::Vector3i> getChunkAndPos(const mk::math::Vector3i world_pos);

	Player* player{};
	bool    wireframe = false;

	mk::Shader chunk_shader;
};
