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

	bool canSeeFaceFrom(mk::math::Vector3f cam_rot_deg, float fov_h, float fov_v, FaceDir dir) {
		float half_fov_h = fov_h / 2.f;
		float half_fov_v = fov_v / 2.f;

		auto [pitch, yaw, _] = cam_rot_deg.bind();
		std::cerr << half_fov_h << " " << yaw << '\n';

		switch (dir) {
		case FaceDir::NORTH:
			if (yaw <= half_fov_h) return false;
			return yaw <= half_fov_h - 5.f;
		case FaceDir::EAST:
			break;
		case FaceDir::SOUTH:
			break;
		case FaceDir::WEST:
			break;
		case FaceDir::UP:
			return pitch <= half_fov_v;
		case FaceDir::DOWN:
			return pitch >= half_fov_v;
		}

		return false;
	}
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
		// But it's really tricky.
		if (faces[dir_id].getSize()) {
			context.texture = getVoxelTexture(type, dir_id);
			target.render(faces[dir_id], context);
		}
	}
}
