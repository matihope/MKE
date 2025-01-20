#pragma once
#include "MKE/Game.hpp"
#include "MKE/Ints.hpp"

#include <utility>

enum class GameItem : u8 {
	AIR,
	GRASS,
	DIRT,
	STONE,
	LOG,
	LEAF,
	BEDROCK,
	DIAMOND_ORE,
	GOLD_ORE,
	_NUM_BLOCKS
};
constexpr u8   VOXEL_TYPES                 = static_cast<u8>(GameItem::_NUM_BLOCKS);
constexpr bool IS_TRANSLUCENT[VOXEL_TYPES] = {
	true,   // AIR
	false,  // GRASS
	false,  // DIRT
	false,  // STONE
	false,  // LOG
	true,   // LEAF
	false,  // BEDROCK
	false,  // DIAMOND_ORE
	false,  // GOLD ORE
};


enum class FaceDir : u8 { NORTH, EAST, SOUTH, WEST, UP, DOWN };
constexpr u8 FACE_DIRS = 6;

constexpr i32 CHUNK_SIZE = 32;

constexpr mk::math::Vector3i getDirVec(const FaceDir dir) {
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

constexpr std::pair<FaceDir, FaceDir> getGrowDir(const FaceDir dir) {
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

constexpr std::pair<mk::math::Vector3i, mk::math::Vector3i> getGrowDirVec(const FaceDir dir) {
	auto [u, r] = getGrowDir(dir);
	return { getDirVec(u), getDirVec(r) };
}

constexpr mk::math::Vector3i getBottomLeftFor(const FaceDir dir) {
	// I think this is pointless...
	switch (dir) {
	case FaceDir::SOUTH:
		return { 0, 0, 0 };
	case FaceDir::WEST:
		return { 0, 0, 1 };
	case FaceDir::EAST:
		return { 1, 0, 0 };
	case FaceDir::NORTH:
		return { 1, 0, 1 };
	case FaceDir::UP:
		return { 0, 1, 0 };
	// case FaceDir::DOWN:
	default:
		return { 0, 0, 1 };
	}
}

void               initTextures(mk::Game& game);
const mk::Texture* getVoxelTexture(GameItem type, FaceDir dir);
const mk::Texture* getVoxelTexture(GameItem type, usize dir_id);
