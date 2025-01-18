#include "World.hpp"

void World::onReady(mk::Game& game) {
	player            = addChild<Player, 10>(game, *this);
	// constexpr int CNT = 7;  // (CNT * 2 + 1) ** 2 * 2
	constexpr int CNT = 2;  // (CNT * 2 + 1) ** 2 * 2
	for (int x = -CNT; x <= CNT; x++) {
		for (int y = 0; y <= 1; y++) {
			for (int z = -CNT; z <= CNT; z++) {
				chunk_list.push_back(Chunk(player->getCamera(), mk::math::Vector3i(x, y, z)));
				chunk_list.back().onReady(game);
				chunks[x][y][z] = &chunk_list.back();
			}
		}
	}
	chunk_shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
}

void World::onEvent(mk::Game& game, const mk::Event& event) {
	if (const auto ev = event.get<mk::Event::KeyPressed>(); ev) {
		if (ev->key == mk::input::KEY::T) wireframe ^= 1;
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
	context.shader = &chunk_shader;
	for (auto& chunk: chunk_list) chunk.drawEntity(target, context, game, getDrawMode(), true);
}

std::pair<Chunk*, mk::math::Vector3i> World::getChunkAndPos(const mk::math::Vector3i world_pos) {
	auto [x, y, z] = (world_pos % CHUNK_SIZE).vec_data;
	if (x < 0) x += CHUNK_SIZE;
	if (y < 0) y += CHUNK_SIZE;
	if (z < 0) z += CHUNK_SIZE;
	const auto ch_x  = mk::math::customDiv(world_pos.x, CHUNK_SIZE);
	const auto ch_y  = mk::math::customDiv(world_pos.y, CHUNK_SIZE);
	const auto ch_z  = mk::math::customDiv(world_pos.z, CHUNK_SIZE);
	Chunk*     chunk = nullptr;
	if (const auto ch = chunks[ch_x][ch_y][ch_z]; ch) chunk = ch;
	return { chunk, { x, y, z } };
}
