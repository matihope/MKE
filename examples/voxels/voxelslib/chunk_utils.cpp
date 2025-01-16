#include "chunk_utils.hpp"

std::pair<FaceDir, FaceDir> getGrowDir(const FaceDir dir) {
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

std::unordered_map<VoxelType, std::array<const mk::Texture*, 6>> textures;

mk::ResPath getTexturePath(const VoxelType type, const FaceDir dir) {
	MK_ASSERT(type == VoxelType::DIRT, "For now expecting dirt");
	std::string path = "dirt_";
	switch (dir) {
	case FaceDir::NORTH:
	case FaceDir::EAST:
	case FaceDir::SOUTH:
	case FaceDir::WEST:
		path += "side";
		break;
	case FaceDir::UP:
		path += "up";
		break;
	case FaceDir::DOWN:
		path += "down";
		break;
	}
	path += ".png";
	return { path };
}

void initTextures(mk::Game& game) {
	static bool init = false;
	if (!init) {
		auto                              type = VoxelType::DIRT;
		std::array<const mk::Texture*, 6> type_textures{};
		for (int i = 0; i < FACE_DIRS; i++) {
			const auto dir   = static_cast<FaceDir>(i);
			auto       path  = getTexturePath(type, dir);
			type_textures[i] = game.resources().getTexture(path);
			game.resources().setTextureSmooth(path, false);
			game.resources().setTextureWrapMethod(path, GL_REPEAT);
		}
		textures.emplace(type, type_textures);
	}
	init = true;
}

const mk::Texture* getVoxelTexture(const VoxelType type, FaceDir dir) {
	return getVoxelTexture(type, static_cast<usize>(dir));
}

const mk::Texture* getVoxelTexture(const VoxelType type, usize dir_id) {
	return textures.at(type)[dir_id];
}
