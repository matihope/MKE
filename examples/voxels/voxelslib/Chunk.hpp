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

struct CubePos {
	usize x = 0;
	usize y = 0;
	usize z = 0;

	static mk::math::Vector3i getDirVec(const FaceDir dir) {
		switch (dir) {
		case FaceDir::NORTH: {
			return { 0, 0, 1 };
		}
		case FaceDir::EAST: {
			return { 1, 0, 0 };
		}
		case FaceDir::SOUTH: {
			return { 0, 0, -1 };
		}
		case FaceDir::WEST: {
			return { -1, 0, 0 };
		}
		case FaceDir::UP: {
			return { 0, 1, 0 };
		}
		// case FaceDir::DOWN:
		default: {
			return { 0, -1, 0 };
		}
		}
	}

	[[nodiscard]]
	std::pair<CubePos, bool> moveDir(const FaceDir dir) const {
		switch (dir) {
		case FaceDir::NORTH: {
			bool flipped = z == CHUNK_SIZE - 1;
			return { { x, y, (z + CHUNK_SIZE + 1) % CHUNK_SIZE }, flipped };
		}
		case FaceDir::EAST: {
			bool flipped = x == CHUNK_SIZE - 1;
			return { { (x + CHUNK_SIZE + 1) % CHUNK_SIZE, y, z }, flipped };
		}
		case FaceDir::SOUTH: {
			bool flipped = z == 0;
			return { { x, y, (z + CHUNK_SIZE - 1) % CHUNK_SIZE }, flipped };
		}
		case FaceDir::WEST: {
			bool flipped = x == 0;
			return { { (x - CHUNK_SIZE - 1) % CHUNK_SIZE, y, z }, flipped };
		}
		case FaceDir::UP: {
			bool flipped = y == CHUNK_SIZE - 1;
			return { { x, (y + CHUNK_SIZE + 1) % CHUNK_SIZE, z }, flipped };
		}
		// case FaceDir::DOWN:
		default: {
			bool flipped = y == 0;
			return { { x, (y + CHUNK_SIZE - 1) % CHUNK_SIZE, z }, flipped };
		}
		}
	}
};

using VoxelFaceArray = mk::VertexArray<VoxelVertex>;

class Chunk final: public mk::WorldEntity3D {
public:
	mk::Shader shader;

	void onReady(mk::Game& game) override {
		buildMeshes(game);
		shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
	}

	void onDraw(
		mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game
	) const override {
		context.transform *= getTransform();
		context.shader = &shader;
		for (auto&& [_, arr]: faces) target.render(arr, context);
	}

private:
	[[nodiscard]]
	bool anyOf(VoxelType type) const {
		for (auto& row: voxels[static_cast<usize>(type)])
			if (row) return true;
		return false;
	}

	void clearFacesOf(const VoxelType type) {
		if (const auto it = faces.find(type); it != faces.end()) faces.erase(it);
	}

	VoxelFaceArray& getFacesOf(const VoxelType type) {
		auto [arr, emplaced] = faces.try_emplace(type, false);
		return arr->second;
	}

	void buildMeshes(mk::Game&) {
		for (usize type_index = 0; type_index < VOXEL_TYPES; ++type_index) {
			if (const auto voxel_type = static_cast<VoxelType>(type_index);
			    anyOf(voxel_type) || true) {
				auto&  voxel_faces = getFacesOf(voxel_type);
				auto&& va          = voxels[type_index];
				// Greedy meshing here.
				// This has to look at the vertices from every direction.
				if (voxel_type == VoxelType::DIRT) {
					// This is temporary
					voxel_faces.setSize(36);
					buildFace({ 0, 0, 0 }, FaceDir::SOUTH, CHUNK_SIZE, CHUNK_SIZE, 0, voxel_faces);
					buildFace({ 0, 0, 32 }, FaceDir::WEST, CHUNK_SIZE, CHUNK_SIZE, 6, voxel_faces);
					buildFace({ 32, 0, 0 }, FaceDir::EAST, CHUNK_SIZE, CHUNK_SIZE, 12, voxel_faces);
					buildFace(
						{ 32, 0, 32 }, FaceDir::NORTH, CHUNK_SIZE, CHUNK_SIZE, 18, voxel_faces
					);
					buildFace({ 0, 32, 0 }, FaceDir::UP, CHUNK_SIZE, CHUNK_SIZE, 24, voxel_faces);
					buildFace({ 0, 0, 32 }, FaceDir::DOWN, CHUNK_SIZE, CHUNK_SIZE, 30, voxel_faces);

					for (int i = 0; i < 36; ++i) {
						voxel_faces(i).color = { mk::Random::getReal(0., 1.),
							                     mk::Random::getReal(0., 1.),
							                     mk::Random::getReal(0., 1.) };
					}
				}
				voxel_faces.save();
			} else {
				clearFacesOf(voxel_type);
			}
		}
	}

	static std::pair<FaceDir, FaceDir> getGrowDir(const FaceDir dir) {
		auto grow_dir_right = FaceDir::EAST;
		auto grow_dir_up    = FaceDir::UP;
		switch (dir) {
		case FaceDir::NORTH: {
			grow_dir_right = FaceDir::WEST;
			break;
		}
		case FaceDir::EAST: {
			grow_dir_right = FaceDir::NORTH;
			break;
		}
		case FaceDir::SOUTH: {
			grow_dir_right = FaceDir::EAST;
			break;
		}
		case FaceDir::WEST: {
			grow_dir_right = FaceDir::SOUTH;
			break;
		}
		case FaceDir::UP: {
			grow_dir_up    = FaceDir::NORTH;
			grow_dir_right = FaceDir::EAST;
			break;
		}
		case FaceDir::DOWN: {
			grow_dir_up    = FaceDir::SOUTH;
			grow_dir_right = FaceDir::EAST;
			break;
		}
		}
		return { grow_dir_up, grow_dir_right };
	}

	static void buildFace(
		mk::math::Vector3i bottom_left,
		const FaceDir      dir,
		const usize        size_x,
		const usize        size_y,
		usize              start,
		VoxelFaceArray&    voxels
	) {
		auto [grow_dir_up, grow_dir_right] = getGrowDir(dir);
		auto               vec_grow_up     = CubePos::getDirVec(grow_dir_up) * size_y;
		auto               vec_grow_right  = CubePos::getDirVec(grow_dir_right) * size_x;
		mk::math::Vector3i top_right       = bottom_left;
		top_right += vec_grow_up;
		top_right += vec_grow_right;
		mk::math::Vector3i bottom_right = bottom_left;
		bottom_right += vec_grow_right;
		mk::math::Vector3i top_left = bottom_left;
		top_left += vec_grow_up;

		voxels(start + 0).position = bottom_left;
		voxels(start + 1).position = bottom_right;
		voxels(start + 2).position = top_right;
		voxels(start + 3).position = bottom_left;
		voxels(start + 4).position = top_right;
		voxels(start + 5).position = top_left;

		std::cerr << "top_right = " << top_right << std::endl;
		std::cerr << "bottom_right = " << bottom_right << std::endl;
		std::cerr << "top_left = " << top_left << std::endl;
		std::cerr << "bottom_left = " << bottom_left << std::endl;
	}

	std::array<std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>, VOXEL_TYPES> voxels{};
	std::map<VoxelType, VoxelFaceArray>                                             faces;
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
