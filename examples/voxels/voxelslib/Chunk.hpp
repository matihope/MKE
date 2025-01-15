#pragma once

#include "VoxelVertex.hpp"
#include "MKE/Game.hpp"
#include "MKE/Primitives/3d/CubePrimitive.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Random.hpp"
#include "MKE/Shaders/SimpleShader.hpp"

enum class VoxelType : u8 { EMPTY, DIRT };
constexpr u8 VOXEL_TYPES = 2;

enum class FaceDir : u8 { NORTH, EAST, SOUTH, WEST, UP, DOWN };
constexpr u8 FACE_DIRS = 6;

constexpr u8 CHUNK_SIZE = 32;


using VoxelFaceArray = mk::VertexArray<VoxelVertex>;

class Chunk final: public mk::WorldEntity3D {
public:
	mk::Shader shader;

	void onReady(mk::Game& game) override;

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override;

private:
	void            clearFacesOf(VoxelType type);
	std::array<VoxelFaceArray, 6>& getFacesOf(VoxelType type);

	void buildMeshes(mk::Game&);

	std::array<std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>, VOXEL_TYPES> voxels{};
	std::map<VoxelType, std::array<VoxelFaceArray, 6>>                              faces{};
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
