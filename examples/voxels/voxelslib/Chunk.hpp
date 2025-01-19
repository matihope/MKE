#pragma once

#include "VoxelTextureFaces.hpp"
#include "chunk_utils.hpp"
#include "MKE/Game.hpp"
#include "MKE/WorldEntity.hpp"

enum class ChunkDrawMode {
	ONLY_OPAQUE,
	ONLY_TRANSLUCENT,
};

class Chunk final: public mk::WorldEntity3D {
public:
	Chunk(mk::Camera3D* camera, mk::math::Vector3i position):
		  int_position(position),
		  camera(camera) {
		setPosition(position.type<float>() * CHUNK_SIZE);
		voxels.fill(VoxelType::AIR);
	}

	void onReady(mk::Game& game) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

	void      setBlock(usize x, usize y, usize z, VoxelType type, bool rebuild = true);
	VoxelType getBlockType(usize x, usize y, usize z) const;

	mk::math::Vector3i getIntPosition() const;

	void setChunkDrawMode(const ChunkDrawMode mode) { chunk_draw_mode = mode; }

private:
	mk::math::Vector3i int_position;
	mk::Camera3D*      camera;
	void               clearFacesOf(VoxelType type);
	VoxelTextureFaces& getFacesOf(VoxelType type);

	ChunkDrawMode chunk_draw_mode = ChunkDrawMode::ONLY_OPAQUE;

	void buildMeshes();

	std::array<VoxelType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels{};
	std::map<VoxelType, VoxelTextureFaces>                      faces{};
};

/*
Idea:

====
Every chunk consists of 64 * 6 layers of faces.
Each Layer has to be reconstructed on the modification of the chunk.
This cannot help us draw fewer triangles, but can help us reduce the number of draw calls.

If only we knew how to draw multiple textures on one rectangle (triangle pair).
How could we get information about a block_kind inside fragment_shader.
====

====
!!!
Each texture has its own VertexArray, and we compose each chunk of faces of a single texture.

====
!!!
Positive z = north
Positive y = UP
Positive x = east


*/
