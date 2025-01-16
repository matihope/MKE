//
// Created by Mateusz Kolpa on 15/01/2025.
//

#include "VoxelTextureFaces.hpp"

#include "Chunk.hpp"
#include "MKE/Math/Vector.hpp"

namespace {
	mk::math::Vector3i getDirVec(const FaceDir dir) {
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

	std::unordered_map<VoxelType, std::array<const mk::Texture*, 6>> textures;

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

void VoxelTextureFaces::addFace(
	mk::math::Vector3i bottom_left, FaceDir dir, usize size_x, usize size_y
) {
	auto [grow_dir_up, grow_dir_right] = getGrowDir(dir);
	const auto vec_grow_up             = getDirVec(grow_dir_up);
	const auto vec_grow_right          = getDirVec(grow_dir_right);

	mk::math::Vector3i top_right = bottom_left;
	top_right += vec_grow_up * size_y;
	top_right += vec_grow_right * size_x;
	mk::math::Vector3i bottom_right = bottom_left;
	bottom_right += vec_grow_right * size_x;
	mk::math::Vector3i top_left = bottom_left;
	top_left += vec_grow_up * size_y;

	auto& voxels = faces[static_cast<usize>(dir)];
	usize start  = voxels.getSize();
	voxels.addVertices(6);
	voxels(start + 0).position   = bottom_left;
	voxels(start + 0).tex_coords = { 1.f * size_x, 1.f * size_y };
	voxels(start + 1).position   = bottom_right;
	voxels(start + 1).tex_coords = { 0.f, 1.f * size_y };
	voxels(start + 2).position   = top_right;
	voxels(start + 2).tex_coords = { 0.f, 0.f };
	voxels(start + 3).position   = bottom_left;
	voxels(start + 3).tex_coords = { 1.f * size_x, 1.f * size_y };
	voxels(start + 4).position   = top_right;
	voxels(start + 4).tex_coords = { 0.f, 0.f };
	voxels(start + 5).position   = top_left;
	voxels(start + 5).tex_coords = { 1.f * size_x, 0.f };
}

void VoxelTextureFaces::save() {
	for (auto& v: faces) v.save();
}

void VoxelTextureFaces::draw(mk::RenderTarget& target, mk::DrawContext context) const {
	for (usize dir_id = 0; dir_id < FACE_DIRS; dir_id++) {
		// Here we also can not call draw if player is facing away.
		if (faces[dir_id].getSize()) {
			context.texture = textures.at(type)[dir_id];
			target.render(faces[dir_id], context);
		}
	}
}
