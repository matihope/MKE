#pragma once
#include "MKE/Game.hpp"
#include "MKE/Ints.hpp"

#include <utility>

enum class VoxelType : u8 { AIR, DIRT };
constexpr u8 VOXEL_TYPES = 2;

enum class FaceDir : u8 { NORTH, EAST, SOUTH, WEST, UP, DOWN };
constexpr u8 FACE_DIRS = 6;

constexpr u8 CHUNK_SIZE = 32;

std::pair<FaceDir, FaceDir> getGrowDir(FaceDir dir);

void               initTextures(mk::Game& game);
const mk::Texture* getVoxelTexture(VoxelType type, FaceDir dir);
const mk::Texture* getVoxelTexture(VoxelType type, usize dir_id);
