#pragma once
#include "Chunk.hpp"
#include "Player.hpp"

#include "MKE/DrawContext.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"

#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"

#include <PerlinNoise.hpp>

class World final: public mk::WorldEntity3D {
	const i32 WORLD_SIZE;  // CHUNK_CNT = (WORLD_SIZE * 2 + 1) ** 2 * 2
	const i32 CHUNK_LAYERS = 4;
	const i32 CHUNK_COUNT  = (WORLD_SIZE * 2 + 1) * (WORLD_SIZE * 2 + 1) * CHUNK_LAYERS;

public:
	explicit World(GameMode player_mode, i32 world_size, std::optional<usize> seed = {});

	void onReady(mk::Game& game) override;

	void onEvent(mk::Game& game, const mk::Event& event) override;

	void onUpdate(mk::Game& game, float dt) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

	Player* player{};
	bool    wireframe = false;
	bool    fog_on    = true;

	float getFogDistance() const;

	void setFogDistance(float fd);

	float FOG_DISTANCE = 20;

	mk::Shader chunk_shader;

	std::pair<Chunk*, mk::math::Vector3i> getChunkAndPos(mk::math::Vector3i world_pos) const;

	i32 getChunkGenHeight(i32 x, i32 z) const;

private:
	siv::PerlinNoise perlin_noise;

	void  addChunk(mk::Game& game, Chunk&& chunk);
	usize getChunkIndex(mk::math::Vector3i coords) const;

	std::list<Chunk> chunk_list;
	// std::unordered_map<i32, std::unordered_map<i32, std::unordered_map<i32, Chunk*>>> chunks;
	// std::map<i32, std::map<i32, std::map<i32, Chunk*>>> chunks;
	std::unordered_map<usize, Chunk*> chunks = {};

	GameMode requested_player_mode;

	const mk::Texture* texture_batch{};
	float time = 0.f;
};

inline void World::setFogDistance(float fd) {
	FOG_DISTANCE = fd;
	chunk_shader.setFloat("FOG_DIST", CHUNK_SIZE * FOG_DISTANCE);
	chunk_shader.setFloat("FOG_DIST_0", CHUNK_SIZE * (FOG_DISTANCE + 3));
}

