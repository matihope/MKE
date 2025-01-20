#pragma once

#include "VoxelTextureFaces.hpp"
#include "chunk_utils.hpp"
#include "MKE/Camera.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"

class World;

enum class ChunkDrawMode {
	ONLY_OPAQUE,
	ONLY_TRANSLUCENT,
};

class Chunk final: public mk::WorldEntity3D {
public:
	Chunk(mk::Camera3D* camera, mk::math::Vector3i position, World& world);

	void onReady(mk::Game& game) override;

	void generateTerrain(mk::Game& game);
	void generateTrees(mk::Game& game);

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

	void     setBlock(usize x, usize y, usize z, GameItem type, bool rebuild = true);
	GameItem getBlockType(usize x, usize y, usize z) const;
	GameItem getBlockType(mk::math::Vector3u pos) const;

	mk::math::Vector3i getIntPosition() const;

	void setChunkDrawMode(const ChunkDrawMode mode) { chunk_draw_mode = mode; }

private:
	World&             world;
	mk::math::Vector3i int_position;
	mk::Camera3D*      camera;

	ChunkDrawMode chunk_draw_mode = ChunkDrawMode::ONLY_OPAQUE;

	void buildMeshes();

	std::array<GameItem, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels{};
	std::unique_ptr<VoxelTextureFaces> faces = nullptr;
};
