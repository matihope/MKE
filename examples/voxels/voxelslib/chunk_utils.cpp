#include "chunk_utils.hpp"


std::unordered_map<GameItem, std::array<const mk::Texture*, 6>> textures;

mk::ResPath getTexturePath(const GameItem type, const FaceDir dir) {
	MK_ASSERT(type != GameItem::AIR, "Invalid block: AIR");
	std::string path;
	if (type == GameItem::GRASS) path = "grass_";
	if (type == GameItem::DIRT) path = "dirt_";
	if (type == GameItem::STONE) path = "stone_";
	if (type == GameItem::LOG) path = "log_";
	if (type == GameItem::LEAF) path = "leaf_";
	if (type == GameItem::BEDROCK) path = "bedrock_";
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
		for (usize type_idx = 1; type_idx < VOXEL_TYPES; ++type_idx) {
			auto                              type = static_cast<GameItem>(type_idx);
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
	}
	init = true;
}

const mk::Texture* getVoxelTexture(const GameItem type, FaceDir dir) {
	return getVoxelTexture(type, static_cast<usize>(dir));
}

const mk::Texture* getVoxelTexture(const GameItem type, usize dir_id) {
	return textures.at(type)[dir_id];
}
