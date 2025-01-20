//
// Created by Mateusz Kolpa on 15/01/2025.
//

#include "VoxelTextureFaces.hpp"

#include "Chunk.hpp"
#include "MKE/Math/Vector.hpp"

namespace {
	bool canSeeFaceFrom(mk::math::Vector3f cam_rot_deg, float fov_h, float fov_v, FaceDir dir) {
		float half_fov_h = fov_h / 2.f;
		float half_fov_v = fov_v / 2.f;

		auto [pitch, yaw, _] = cam_rot_deg.vec_data;
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

void VoxelTextureFaces::clearFaces() { faces.setSize(0); }

void VoxelTextureFaces::addFace(
	mk::math::Vector3i bottom_left, FaceDir dir, usize size_x, usize size_y, GameItem type
) {
	bottom_left += getBottomLeftFor(dir);  // Offset the face position

	const auto [vec_grow_up, vec_grow_right] = getGrowDirVec(dir);


	mk::math::Vector3i top_right = bottom_left;
	top_right += vec_grow_up * size_y;
	top_right += vec_grow_right * size_x;
	mk::math::Vector3i bottom_right = bottom_left;
	bottom_right += vec_grow_right * size_x;
	mk::math::Vector3i top_left = bottom_left;
	top_left += vec_grow_up * size_y;

	const usize start = faces.getSize();
	faces.addVertices(6);
	faces(start + 0).encodeData(bottom_left, dir, type);
	faces(start + 1).encodeData(bottom_right, dir, type);
	faces(start + 2).encodeData(top_right, dir, type);
	faces(start + 3).encodeData(bottom_left, dir, type);
	faces(start + 4).encodeData(top_right, dir, type);
	faces(start + 5).encodeData(top_left, dir, type);
}

void VoxelTextureFaces::save() {
	faces.save();
}

void VoxelTextureFaces::draw(mk::RenderTarget& target, mk::DrawContext context) const {
	// for (usize dir_id = 0; dir_id < FACE_DIRS; dir_id++) {
	// 	// Here we also can not call draw if player is facing away.
	// 	// But it's really tricky.
	// 	if (faces[dir_id].getSize()) {
	// 		context.texture = getVoxelTexture(type, dir_id);
	// 		target.render(faces[dir_id], context);
	// 	}
	// }
	target.render(faces, context);
}
