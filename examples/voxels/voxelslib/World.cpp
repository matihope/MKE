#include "World.hpp"

#include "MKE/Random.hpp"

World::World(const GameMode player_mode, const i32 world_size, const std::optional<usize> seed):
	  perlin_noise(seed ? seed.value() : mk::Random::getInt(0, 255)),
	  WORLD_SIZE(world_size),
	  requested_player_mode(player_mode) {}

void World::onReady(mk::Game& game) {
	std::cerr << "Creating a world of " << CHUNK_COUNT << " chunks, "
			  << (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) * CHUNK_COUNT << " voxels..." << std::endl;
	chunks.resize(CHUNK_COUNT);
	chunk_shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));

	constexpr auto bg_color = mk::Color(182, 242, 243);
	game.setClearColor(bg_color);
	chunk_shader.setColor("BG_COLOR", bg_color);
	chunk_shader.setFloat("FOG_DIST", CHUNK_SIZE * FOG_DISTANCE);
	chunk_shader.setFloat("FOG_DIST_0", CHUNK_SIZE * (FOG_DISTANCE + 1));
	chunk_shader.setBool("FOG_ON", fog_on);

	player = addChild<Player, 10>(game, *this, requested_player_mode);

	for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
		for (int y = 0; y < CHUNK_LAYERS; y++)
			for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++)
				addChunk(game, Chunk(player->getCamera(), mk::math::Vector3i(x, y, z)));
	}

	for (auto& chunk: chunk_list) chunk.generateTerrain(game, *this);
	for (auto& chunk: chunk_list) chunk.generateTrees(game, *this);
	player->initialReposition();
}

void World::onEvent(mk::Game& game, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::T) wireframe ^= 1;
		if (ev->key == mk::input::KEY::F) chunk_shader.setBool("FOG_ON", fog_on ^= 1);
		if (ev->key == mk::input::KEY::R)
			chunk_shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
	}
}

void World::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const {
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Here we should not draw chunks that player cannot see.
	context.shader       = &chunk_shader;
	constexpr float DIST = CHUNK_SIZE * (FOG_DISTANCE + 3);

	// Drawing chunks...
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for (auto& chunk: chunk_list) {
		const_cast<Chunk&>(chunk).setChunkDrawMode(ChunkDrawMode::ONLY_OPAQUE);
		if (auto pos = chunk.getPosition();
		    (player->getCamera()->getPosition() - pos).lengthSquared() <= DIST * DIST)
			chunk.drawEntity(target, context, game, getDrawMode(), true);
	}
	for (auto& chunk: chunk_list) {
		const_cast<Chunk&>(chunk).setChunkDrawMode(ChunkDrawMode::ONLY_TRANSLUCENT);
		if (auto pos = chunk.getPosition();
		    (player->getCamera()->getPosition() - pos).lengthSquared() <= DIST * DIST)
			chunk.drawEntity(target, context, game, getDrawMode(), true);
	}
}

std::pair<Chunk*, mk::math::Vector3i>
	World::getChunkAndPos(const mk::math::Vector3i world_pos) const {
	auto [x, y, z] = (world_pos % CHUNK_SIZE).vec_data;
	if (x < 0) x += CHUNK_SIZE;
	if (y < 0) y += CHUNK_SIZE;
	if (z < 0) z += CHUNK_SIZE;
	const auto ch_x  = mk::math::customDiv(world_pos.x, CHUNK_SIZE);
	const auto ch_y  = mk::math::customDiv(world_pos.y, CHUNK_SIZE);
	const auto ch_z  = mk::math::customDiv(world_pos.z, CHUNK_SIZE);
	const auto idx   = getChunkIndex({ ch_x, ch_y, ch_z });
	Chunk*     chunk = nullptr;
	if (idx < chunks.size()) chunk = chunks[idx];
	return { chunk, { x, y, z } };
}

usize World::getChunkIndex(const mk::math::Vector3i coords) const {
	if (coords.x < -WORLD_SIZE || coords.x > WORLD_SIZE) return std::numeric_limits<usize>::max();
	if (coords.y < 0 || coords.y >= CHUNK_LAYERS) return std::numeric_limits<usize>::max();
	if (coords.z < -WORLD_SIZE || coords.z > WORLD_SIZE) return std::numeric_limits<usize>::max();
	const i32  WIDTH  = WORLD_SIZE * 2 + 1;
	const auto ch_pos = coords + mk::math::Vector3i{ WORLD_SIZE, 0, WORLD_SIZE };
	return static_cast<usize>(ch_pos.y * WIDTH * WIDTH + ch_pos.x * WIDTH + ch_pos.z);
}

void World::addChunk(mk::Game& game, Chunk&& chunk) {
	const auto idx = getChunkIndex(chunk.getIntPosition());
	chunk_list.push_back(std::move(chunk));
	chunks.at(idx) = &chunk_list.back();
	chunk_list.back().onReady(game);
}

i32 World::getChunkGenHeight(const i32 x, const i32 z) const {
	double value = perlin_noise.octave2D_01(x * 0.01, z * 0.01, 4);

	constexpr double mountains_level = 0.8;
	if (value >= mountains_level) value *= std::pow(1 + value - mountains_level, 3);
	return value * CHUNK_SIZE + 3;
}
