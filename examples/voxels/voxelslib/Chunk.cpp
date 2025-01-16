#include "Chunk.hpp"

namespace {
	bool anyOf(auto voxels, VoxelType type) {
		for (auto& val: voxels[static_cast<usize>(type)])
			if (val) return true;
		return false;
	}
}

void Chunk::onReady(mk::Game& game) {
	voxels[static_cast<usize>(VoxelType::DIRT)][0] = true;
	voxels[static_cast<usize>(VoxelType::DIRT)][1] = true;
	voxels[static_cast<usize>(VoxelType::DIRT)][3] = true;
	buildMeshes(game);
	shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
}

void Chunk::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const {
	context.transform *= getTransform();
	context.shader = &shader;
	// Here we should not draw chunks that player cannot see.
	for (const auto& arr: faces | std::views::values) target.render(arr, context);
}

void Chunk::clearFacesOf(const VoxelType type) {
	if (const auto it = faces.find(type); it != faces.end()) faces.erase(it);
}

VoxelTextureFaces& Chunk::getFacesOf(const VoxelType type) {
	auto [obj, _] = faces.try_emplace(type, type, camera);
	return obj->second;
}

void Chunk::buildMeshes(mk::Game&) {
	for (usize type_index = 0; type_index < VOXEL_TYPES; ++type_index) {
		const auto voxel_type = static_cast<VoxelType>(type_index);
		if (voxel_type == VoxelType::AIR) continue;
		if (anyOf(voxels, voxel_type)) {
			auto& voxel_faces = getFacesOf(voxel_type);
			// voxel_faces.buildFaces();
			// auto&& va = voxels[type_index];
			// Greedy meshing here.
			// // This has to look at the vertices from every direction.
			if (voxel_type == VoxelType::DIRT) {
				// This is temporary
				voxel_faces.addFace({ 0, 0, 0 }, FaceDir::SOUTH, CHUNK_SIZE, CHUNK_SIZE);
				voxel_faces.addFace({ 0, 0, 32 }, FaceDir::WEST, CHUNK_SIZE, CHUNK_SIZE);
				voxel_faces.addFace({ 32, 0, 0 }, FaceDir::EAST, CHUNK_SIZE, CHUNK_SIZE);
				voxel_faces.addFace({ 32, 0, 32 }, FaceDir::NORTH, CHUNK_SIZE, CHUNK_SIZE);
				voxel_faces.addFace({ 0, 32, 0 }, FaceDir::UP, CHUNK_SIZE, CHUNK_SIZE);
				voxel_faces.addFace({ 0, 0, 32 }, FaceDir::DOWN, CHUNK_SIZE, CHUNK_SIZE);
			}
			// for (usize dir_idx = 0; dir_idx < FACE_DIRS; ++dir_idx)
			// 	const auto face_dir = static_cast<FaceDir>(dir_idx);
			// 	for (usize x = 0; x < CHUNK_SIZE; ++x) {
			// 		for (usize y = 0; y < CHUNK_SIZE; ++y) {
			// 			for (usize z = 0; z < CHUNK_SIZE; ++z) {
			//
			// 			}
			// 		}
			// 	}
			//
			// }
			voxel_faces.save();
		} else {
			clearFacesOf(voxel_type);
		}
	}
}
