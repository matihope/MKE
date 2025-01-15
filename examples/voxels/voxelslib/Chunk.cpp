#include "Chunk.hpp"

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

	bool anyOf(auto voxels, VoxelType type) {
		for (auto& val: voxels[static_cast<usize>(type)])
			if (val) return true;
		return false;
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

	void buildFace(
		mk::math::Vector3i bottom_left,
		const FaceDir      dir,
		const usize        size_x,
		const usize        size_y,
		usize              start,
		VoxelFaceArray&    voxels
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

		voxels(start + 0).position   = bottom_left;
		voxels(start + 0).tex_coords = { 0.f, 0.f };
		voxels(start + 1).position   = bottom_right;
		voxels(start + 1).tex_coords = { 1.f * size_x, 0.f };
		voxels(start + 2).position   = top_right;
		voxels(start + 2).tex_coords = { 1.f * size_x, 1.f * size_y };
		voxels(start + 3).position   = bottom_left;
		voxels(start + 3).tex_coords = { 0.f, 0.f };
		voxels(start + 4).position   = top_right;
		voxels(start + 4).tex_coords = { 1.f * size_x, 1.f * size_y };
		voxels(start + 5).position   = top_left;
		voxels(start + 5).tex_coords = { 0.f, 1.f * size_y };
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

	void loadTextures(mk::Game& game) {
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

	std::array<VoxelFaceArray, 6> createDefaultVoxelFaces() {
		return { false, false, false, false, false, false };
	}
}

void Chunk::onReady(mk::Game& game) {
	voxels[static_cast<usize>(VoxelType::DIRT)][0] = true;
	loadTextures(game);
	buildMeshes(game);
	shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
}

void Chunk::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game) const {
	context.transform *= getTransform();
	context.shader = &shader;
	for (auto&& [tp, arr]: faces) {
		for (usize dir_id = 0; dir_id < FACE_DIRS; dir_id++) {
			context.texture = textures.at(tp)[dir_id];
			target.render(arr[dir_id], context);
		}
	}
}

void Chunk::clearFacesOf(const VoxelType type) {
	if (const auto it = faces.find(type); it != faces.end()) faces.erase(it);
}

std::array<VoxelFaceArray, 6>& Chunk::getFacesOf(const VoxelType type) {
	return faces[type];
}

void Chunk::buildMeshes(mk::Game&) {
	for (usize type_index = 0; type_index < VOXEL_TYPES; ++type_index) {
		const auto voxel_type = static_cast<VoxelType>(type_index);
		if (anyOf(voxels, voxel_type)) {
			auto& voxel_faces = getFacesOf(voxel_type);
			for (auto& a: voxel_faces) a.setSize(6);
			// auto&& va = voxels[type_index];
			// Greedy meshing here.
			// This has to look at the vertices from every direction.
			if (voxel_type == VoxelType::DIRT) {
				// This is temporary
				buildFace(
					{ 0, 0, 0 },
					FaceDir::SOUTH,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::SOUTH)]
				);
				buildFace(
					{ 0, 0, 32 },
					FaceDir::WEST,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::WEST)]
				);
				buildFace(
					{ 32, 0, 0 },
					FaceDir::EAST,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::EAST)]
				);
				buildFace(
					{ 32, 0, 32 },
					FaceDir::NORTH,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::NORTH)]
				);
				buildFace(
					{ 0, 32, 0 },
					FaceDir::UP,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::UP)]
				);
				buildFace(
					{ 0, 0, 32 },
					FaceDir::DOWN,
					CHUNK_SIZE,
					CHUNK_SIZE,
					0,
					voxel_faces[static_cast<usize>(FaceDir::DOWN)]
				);
			}
			for (auto& v: voxel_faces) {
				for (usize i = 0; i < 6; i++) v(i).color = mk::Colors::WHITE;
				v.save();
			}
		} else {
			clearFacesOf(voxel_type);
		}
	}
}
