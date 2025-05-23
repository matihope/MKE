#include "World.hpp"

#include "MKE/Random.hpp"

World::World(const GameMode player_mode, const i32 world_size, const std::optional<usize> seed):
	  WORLD_SIZE(world_size),
	  perlin_noise(seed ? seed.value() : mk::Random::getInt(0, 255)),
	  requested_player_mode(player_mode) {}

void World::onReady(mk::Game& game) {
	std::cerr << "Creating a world of " << CHUNK_COUNT << " chunks, "
			  << (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) * CHUNK_COUNT << " voxels..." << std::endl;
	// chunks.resize(CHUNK_COUNT);
	chunk_shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));

	texture_batch = game.resources().getTexture("texture_batch.png");
	game.resources().setTextureSmooth("texture_batch.png", false);

	constexpr auto bg_color = mk::Color(182, 242, 243);
	game.setClearColor(bg_color);
	chunk_shader.setColor("BG_COLOR", bg_color);
	setFogDistance(FOG_DISTANCE);
	chunk_shader.setFloat("TEXTURE_WIDTH", texture_batch->getSize().x);
	chunk_shader.setFloat("TEXTURE_HEIGHT", texture_batch->getSize().y);
	chunk_shader.setBool("FOG_ON", fog_on);

	player = addChild<Player, 10>(game, *this, requested_player_mode);

	for (int x = -WORLD_SIZE; x <= WORLD_SIZE; x++) {
		for (int y = 0; y < CHUNK_LAYERS; y++)
			for (int z = -WORLD_SIZE; z <= WORLD_SIZE; z++)
				addChunk(game, Chunk(player->getCamera(), mk::math::Vector3i(x, y, z), *this));
	}

	for (auto& chunk: chunk_list) chunk.generateTerrain(game);
	for (auto& chunk: chunk_list) chunk.generateTrees(game);


	player->initialReposition();
}

void World::onEvent(mk::Game&, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::T) wireframe ^= 1;
		if (ev->key == mk::input::KEY::F) chunk_shader.setBool("FOG_ON", fog_on ^= 1);
		if (ev->key == mk::input::KEY::R)
			chunk_shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
	}
}

void World::onUpdate(mk::Game&, const float dt) {
	time += dt;
	chunk_shader.setFloat("TIME", time);
}

void World::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const {
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Here we should not draw chunks that player cannot see.
	context.shader  = &chunk_shader;
	context.texture = texture_batch;

	const float RENDER_DISTANCE = CHUNK_SIZE * (FOG_DISTANCE + std::sqrt(3) * 3);

	// Drawing chunks...
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	constexpr auto cmp = [](auto p1, auto p2) { return p1.first < p2.first; };
	std::priority_queue<
		std::pair<float, const Chunk*>,
		std::vector<std::pair<float, const Chunk*>>,
		decltype(cmp)>
		chunk_queue(cmp);

	for (auto& chunk: chunk_list) {
		const auto chunk_center = chunk.getPosition().type<float>() + CHUNK_SIZE / 2.0;
		const auto dist_vec     = (player->getCamera()->getPosition() - chunk_center);
		const auto dist         = dist_vec.lengthSquared();
		if (dist <= RENDER_DISTANCE * RENDER_DISTANCE) chunk_queue.emplace(dist, &chunk);
	}

	while (!chunk_queue.empty()) {
		auto [_, chunk] = chunk_queue.top();
		chunk->drawEntity(target, context, game, getDrawMode(), true);
		chunk_queue.pop();
	}
}

float World::getFogDistance() const { return FOG_DISTANCE; }

std::pair<Chunk*, mk::math::Vector3i> World::getChunkAndPos(const mk::math::Vector3i world_pos
) const {
	auto [x, y, z] = (world_pos % CHUNK_SIZE).vec_data;
	if (x < 0) x += CHUNK_SIZE;
	if (y < 0) y += CHUNK_SIZE;
	if (z < 0) z += CHUNK_SIZE;
	const auto ch_x  = mk::math::customDiv(world_pos.x, CHUNK_SIZE);
	const auto ch_y  = mk::math::customDiv(world_pos.y, CHUNK_SIZE);
	const auto ch_z  = mk::math::customDiv(world_pos.z, CHUNK_SIZE);
	const auto idx   = getChunkIndex({ ch_x, ch_y, ch_z });
	Chunk*     chunk = nullptr;
	if (idx < chunks.size()) chunk = chunks.at(idx);
	return { chunk, { x, y, z } };
}

usize World::getChunkIndex(const mk::math::Vector3i coords) const {
	if (coords.x < -WORLD_SIZE || coords.x > WORLD_SIZE) return std::numeric_limits<usize>::max();
	if (coords.y < 0 || coords.y >= CHUNK_LAYERS) return std::numeric_limits<usize>::max();
	if (coords.z < -WORLD_SIZE || coords.z > WORLD_SIZE) return std::numeric_limits<usize>::max();
	const i32  WIDTH  = WORLD_SIZE * 2 + 1;
	const auto ch_pos = coords + mk::math::Vector3i{ WORLD_SIZE, 0, WORLD_SIZE };
	return ch_pos.y * WIDTH * WIDTH + ch_pos.x * WIDTH + ch_pos.z;
}

void World::addChunk(mk::Game& game, Chunk&& chunk) {
	const auto idx = getChunkIndex(chunk.getIntPosition());
	chunk_list.push_back(std::move(chunk));
	chunks[idx] = &chunk_list.back();
	chunk_list.back().onReady(game);
}

i32 World::getChunkGenHeight(const i32 x, const i32 z) const {
	double value = perlin_noise.octave2D_01(x * 0.01, z * 0.01, 4);

	constexpr double mountains_level = 0.8;
	if (value >= mountains_level) value *= std::pow(1 + value - mountains_level, 4);
	return value * CHUNK_SIZE * 2 + 3;
}
