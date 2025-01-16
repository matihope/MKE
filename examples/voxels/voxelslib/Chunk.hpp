#pragma once

#include "VoxelTextureFaces.hpp"
#include "MKE/Game.hpp"
#include "MKE/Primitives/3d/CubePrimitive.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Random.hpp"
#include "MKE/Shaders/SimpleShader.hpp"

constexpr u8 CHUNK_SIZE = 32;

class Chunk final: public mk::WorldEntity3D {
public:
	mk::Shader shader;

	void onReady(mk::Game& game) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

private:
	void               clearFacesOf(VoxelType type);
	VoxelTextureFaces& getFacesOf(VoxelType type);

	void buildMeshes(mk::Game&);

	std::array<std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>, VOXEL_TYPES> voxels{};
	std::map<VoxelType, VoxelTextureFaces>                                          faces{};
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
Before drawing each chunk use uniform to set its position in the world and offset it in the shader.

====
!!!
Positive z = north
Positive y = UP
Positive x = east


*/
